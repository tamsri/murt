from murt.window import Window
from murt.utils.generator import SceneGenerator

a = SceneGenerator(888, component_path="./assets/components")
a.generate()
window = Window()
window.scene = a.sceneComponents
window.run()
