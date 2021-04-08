import pyglet
from pyglet.window import Window, key
from pyglet.gl import *
from pywavefront import visualization, Wavefront


class MurtWindow(Window):

    def __init__(self):
        self.cam_pos = [-20, -55, -135]
        self.cam_rot = [20, 0, 0]
        self.win_size = [640, 480]
        super().__init__(width=self.win_size[0],
                         height=self.win_size[1],
                         resizable=True)
        self.pressed_key = {}
        self.scene = None
        self.lines_set = []

    def load_scene(self, file_path):
        self.scene = Wavefront(file_path,
                               collect_faces=True,
                               strict=False)

    def on_resize(self, width, height):
        self.win_size = [width, height]
        glViewport(0, 0, width, height)

    def key_updates():
        pass

    @staticmethod
    def draw_line(lines):
        points = lines['points']
        color = lines['color']
        glBegin(GL_LINES)
        for point in points:
            glVert3f(*point)
        glColor4f(*color)
        glEnd()
        glLineWidth(5)

    def on_draw(self):
        self.clear()
        # Projection
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(60., current_width / float(current_height), .1, 1000.)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        # Translate Camera

        # Draw Scene
        if self.scene is not None:
            visualization.draw(self.scene)
        # Draw Lines
        for lines in self.lines_set:
            draw_lines(lines)
