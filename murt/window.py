import pyglet
from pyglet.window import Window, key
from pyglet.gl import *
from pywavefront import visualization, Wavefront
from murt.utils import objreader
from murt.object import Object
from murt.generator import SceneGenerator
import numpy as np


class MurtWindow(Window):

    def __init__(self):
        self.cam_pos = [0,  0, -255]
        self.cam_rot = [33, -230, 0]
        self.win_size = [640, 480]
        super().__init__(width=self.win_size[0],
                         height=self.win_size[1],
                         resizable=True)
        self.pressed_keys = {}
        self.scene = []
        self.lines_set = []
        glClearColor(0.9, 0.9, 0.875, 1)
        self.alive = True
        self.zoom = 66

    def load_scene(self, file_path):
        self.scene.append(Object(file_path))

    def on_resize(self, width, height):
        self.win_size = [width, height]
        glViewport(0, 0, width, height)

    def on_mouse_drag(self, x, y, dx, dy, buttons, modifieres):
        self.cam_rot[1] += dx
        self.cam_rot[0] -= dy

    def on_mouse_scroll(self, x, y, scroll_x, scroll_y):
        self.zoom = max(20, self.zoom+scroll_y)
        self.zoom = min(120, self.zoom+scroll_y)

    def key_updates(self):
        if key.W in self.pressed_keys:
            self.cam_pos[2] += 1
        if key.S in self.pressed_keys:
            self.cam_pos[2] -= 1
        if key.A in self.pressed_keys:
            self.cam_pos[0] += 1
        if key.D in self.pressed_keys:
            self.cam_pos[0] -= 1
        if key.LSHIFT in self.pressed_keys:
            self.cam_pos[1] += 1
        if key.LCTRL in self.pressed_keys:
            self.cam_pos[1] -= 1
        if key.Z in self.pressed_keys:
            self.cam_rot[0] += 1
        if key.X in self.pressed_keys:
            self.cam_rot[0] -= 1
        if key.Q in self.pressed_keys:
            self.cam_rot[1] += 1
        if key.E in self.pressed_keys:
            self.cam_rot[1] -= 1

    def on_close(self):
        self.alive = False

    def on_key_press(self, pressed_key, modifier):
        if pressed_key == key.ESCAPE:
            self.alive = False
        self.pressed_keys[pressed_key] = True

    def on_key_release(self, pressed_key, modifier):

        if pressed_key == key.F:
            print(self.cam_pos)
            print(self.cam_rot)
            print(self.zoom)
        if pressed_key == key.R:
            self.cam_pos = [0, 0, -180]
            self.cam_rot = [90, -180, 0]
            self.zoom = 100

        try:
            del self.pressed_keys[pressed_key]
        except:
            pass

    def draw_lines(self, lines):
        points = lines['points']
        color = lines['color']
        glBegin(GL_LINE_STRIP)
        glColor4f(*color)
        for point in points:
            glVertex3f(*point)
        glEnd()
        glLineWidth(2)

    def on_draw(self):
        self.render()

    def render(self):
        self.clear()
        self.key_updates()  # update pressed keys
        # Projection
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(self.zoom, self.win_size[0] /
                       float(self.win_size[1]), .1, 1000)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        # Translate Camera
        glTranslatef(*self.cam_pos)
        glRotatef(self.cam_rot[0], 1.0, 0.0, 0.0)
        glRotatef(self.cam_rot[1], 0.0, 1.0, 0.0)
        glRotatef(self.cam_rot[2], 0.0, 0.0, 1.0)
        # Draw Scene
        glEnable(GL_DEPTH_TEST)
        for scene in self.scene:
            scene.draw()
        # Draw Lines
        for lines in self.lines_set:
            self.draw_lines(lines)

        self.flip()

    def custom_render(self):
        self.dispatch_events()
        self.render()

    def run(self):
        while self.alive:
            event = self.dispatch_events()
            self.render()
