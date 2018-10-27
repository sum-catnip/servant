import servant

print('waddup pimps from testmod1')

class TestCat(servant.Category):
    def __init__(self):
        print('init called in testcat')
        caps = [
            servant.Capability("testcap1", self.testcap1, [ servant.Text() ]),
            servant.Capability("testcap2", self.testcap2, [ servant.Text(), servant.Text() ]),
        ]
        super().__init__("testcat", caps)
    
    def testcap1(self, text):
        print('testcap1 called with {}'.format(text.value))

    def testcap2(self, text1, text2):
        print('testcap2 called with {}, {}'.format(text1.value, text2.value))

__module__.add(TestCat())