import ctypes
import threading

import servant
import support

print('waddup pimps from testmod1')

class TestCat(support.Category):
    def __init__(self):
        print('init called in testcat')
        super().__init__("testcat", "testcat")

    def open_msgbox(self, title, msg):
        MessageBox = ctypes.windll.user32.MessageBoxW
        MessageBox(None, msg, title, 0)

    @support.capability('Message Box', servant.Text("title"), servant.Text("message"))
    def message_box(self, title, message):
        threading.Thread(
            target=self.open_msgbox,
            args=(title.value, message.value)
        ).start()
        return servant.Result(servant.Result.OK, "yay")

    @support.capability('throw exception', servant.Text("text"))
    def throw_exception(self, text):
        raise EnvironmentError(text)

cat = TestCat()
__module__.add(cat)