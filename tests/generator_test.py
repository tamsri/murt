from murt.window import Window
from murt.utils.generator import SceneGenerator

a = SceneGenerator(888)
a.generate()
window = Window()
window.scene = a.sceneComponents
window.run()
