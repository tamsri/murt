from murt.generator import SceneGenerator
from murt.window import MurtWindow


a = SceneGenerator(888)
a.Generate()
window = MurtWindow()
window.scene = a.sceneComponents
a.GetTriangles()
window.run()
