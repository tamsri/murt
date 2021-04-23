from murt import scene_generator, mwindow

a = scene_generator(888)
a.generate()
window = mwindow()
window.scene = a.sceneComponents
window.run()
