import pyglet
from pyglet.window import Window, key
import pywavefront
from pywavefront import visualization
import ctypes
from pyglet.gl import *
scene = pywavefront.Wavefront(
    './assets/display_poznan.obj', collect_faces=True, strict=False)

current_width = 640
current_height = 480
current_pos = [-20, -55, -135]
current_rot = [20, 0, 0]
window = Window(width=current_width, height=current_height, resizable=True)


def drawLine(start, end, color):
    glBegin(GL_LINES)
    glVertex3f(*start)
    glVertex3f(*end)
    glColor4f(*color)
    glEnd()
    glLineWidth(10)


@window.event
def on_resize(width, height):
    global current_width, current_height
    current_width = width
    current_height = height
    glViewport(0, 0, width, height)

def key_update():
    

@window.event
def on_draw():
    window.clear()
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60., current_width / float(current_height), .1, 1000.)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    glTranslatef(*current_pos)
    glRotatef(*current_rot, 0)
    glEnable(GL_DEPTH_TEST)
    visualization.draw(scene)
    drawLine((-10, 20, 100), (100, 50, -10), (1, 0, 0, 0, 1))


@ window.event
def on_key_press(symbol, modifiers):
    global current_pos, current_rot
    if symbol == key.W:
        current_pos[2] += 5
    if symbol == key.S:
        current_pos[2] -= 5
    if symbol == key.A:
        current_pos[0] += 5
    if symbol == key.D:
        current_pos[0] -= 5
    if symbol == key.LSHIFT:
        current_pos[1] += 5
    if symbol == key.LCTRL:
        current_pos[1] -= 5
    if symbol == key.Q:
        current_rot[0] += 10
    if symbol == key.E:
        current_rot[0] -= 10

    print(f'{current_pos} , {current_rot}')


pyglet.app.run()
