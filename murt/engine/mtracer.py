
import numpy as np
import pandas as pd

from murt import core, calculator

from murt.utils import objreader


class tracer:
    def __init__(self, scene_path=None):
        if scene_path is not None:
            vertices, triangles = objreader.read(scene_path)
            self.core = core.Tracer(vertices, triangles)
        else:
            self.core = None

    def load_scene(self, vertices, triangles):
        if self.core is not None:
            del self.core
        self.core = core.Tracer(vertices, triangles)

    def is_outdoor(self, pos):
        pos = np.array(pos)
        assert pos.shape[0] == 3
        pos = np.array(pos).astype('float32')
        return self.core.isOutdoor(pos) != 0

    def hit_nearest(self, fromPos, toPos):
        fromPos = np.array(fromPos)
        assert fromPos.shape[0] == 3
        toPos = np.array(toPos)
        assert toPos.shape[0] == 3
        fromPos = fromPos.astype('float32')
        toPos = toPos.astype('float32')
        distance = self.core.hitNearest(fromPos, toPos)
        return distance

    def trace(self, tx_pos, rx_pos):
        if self.core is None:
            return None
        tx_pos = np.array(tx_pos)
        rx_pos = np.array(rx_pos)
        assert rx_pos.shape[0] == 3
        assert tx_pos.shape[0] == 3
        tx_pos = tx_pos.astype('float32')
        rx_pos = rx_pos.astype('float32')
        result = self.core.trace(tx_pos, rx_pos)
        return result

    def get_depth_map(self, x_min, x_max, x_n, z_min, z_max, z_n):
        assert x_min < x_max
        assert z_min < z_max
        assert (x_n > 0) and (z_n > 0)

        upper_y = 1000
        below_y = -1000
        data_depth = []
        data_frame = None

        for current_x in np.linspace(x_min, x_max, x_n):
            for current_z in np.linspace(z_min, z_max, z_n):
                from_pos = [current_x, upper_y, current_z]
                to_pos = [current_x, below_y, current_z]
                distance = self.hit_nearest(from_pos, to_pos)
                # in case if there is no ground, skip
                if distance < 0:
                    depth = 0.0
                else:
                    depth = upper_y-distance
                data_row = {'x': current_x, 'z': current_z, 'depth': depth}
                # print(distance)
                data_depth.append(data_row)

        data_frame = pd.DataFrame(data_depth)
        return data_frame

    def get_traced_volume(self,
                          x_min, x_max, x_n,
                          y_min, y_max, y_n,
                          z_min, z_max, z_n,
                          tx_pos, tx_freq, mat_perm, max_thread=0):
        # check input
        assert x_min < x_max and x_n > 0
        assert y_min < y_max and y_n > 0
        assert z_min < z_max and z_n > 0
        tx_pos = np.array(tx_pos)
        assert tx_pos.shape[0] == 3
        tx_pos = tx_pos.astype('float32')
        tx_freq = float(tx_freq)
        mat_perm = float(mat_perm)
        max_thread = int(max_thread)
        # trace
        results = self.core.traceVolume(x_min, x_max, x_n,
                                        y_min, y_max, y_n,
                                        z_min, z_max, z_n,
                                        tx_pos, tx_freq, mat_perm,
                                        max_thread)
        # prepare data frame
        data_frame = None
        prepared_results = []
        assert results is not None
        for result in results:
            prepared_result = {'x': result[0],
                               'y': result[1],
                               'z': result[2],
                               'total_loss': result[3]}
            prepared_results.append(prepared_result)
        data_frame = pd.DataFrame(prepared_results)
        return data_frame

    @staticmethod
    def result_to_lines(results, tx_pos, rx_pos):
        lines = []
        for result in results:
            line = None
            if result[0] == 1:
                line = {'points': [tuple(tx_pos), tuple(rx_pos)],
                        'color': (0, 0.7, 0.3, 1)}
            if result[0] == 2:
                line = {'points': [tuple(tx_pos)], 'color': (0, 0, .9, 1)}
                for point in result[1]:
                    line['points'].append(tuple(point))
                line['points'].append(tuple(rx_pos))
            if result[0] == 3:
                line = {'points': [tuple(tx_pos), tuple(result[1]), tuple(rx_pos)],
                        'color': (0.7, 0.4, 0.7, 1)}
            if line is not None:
                lines.append(line)
        return lines

    @staticmethod
    def get_total_loss(tx_pos, rx_pos, results, tx_freq=2.4e9, material_permittivity=5.31):
        if results is None:
            return None

        losses = {'total_dB': None, 'signals': []}
        total_linear = 0
        for result in results:
            if result[0] == 1:
                loss_dB, delay = calculator.directLoss(tx_pos, rx_pos, tx_freq)
            elif result[0] == 2:
                edges = list(map(list, result[1]))
                loss_dB, delay = calculator.diffractLoss(
                    tx_pos, rx_pos, edges, tx_freq)
            elif result[0] == 3:
                refPos = list(result[1])
                loss_dB, delay = calculator.reflectLoss(tx_pos, rx_pos,
                                                        refPos, tx_freq,
                                                        material_permittivity)
            else:
                return None
            losses['signals'].append([loss_dB, delay])
            total_linear += np.power(10, -loss_dB/10)

        losses['total_dB'] = -10*np.log10(total_linear)
        return losses
