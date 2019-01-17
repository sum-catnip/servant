import inspect
import servant

print('waddup pimps from testmod1')

def capability(name, *args):
    def decorator(func): 
        func.capability_name = name
        func.capability_args = args
        return func
    return decorator


class Category(servant.Category):
    def __init__(self, name, id_):
        # need to do this before getmembers to prevent a MemoryError
        super().__init__(name, id_)
        for item in inspect.getmembers(self):
            value = item[1]
            if hasattr(value, 'capability_name') and hasattr(value, 'capability_args'):
                self.add(servant.Capability(value.capability_name, value, list(value.capability_args)))


class TestCat(Category):
    def __init__(self):
        print('init called in testcat')
        super().__init__("testcat", "testcat")
    
    @capability('testcap 1', servant.Text("kek"))
    def testcap1(self, text):
        print('testcap1 called with {}'.format(text.value))

    @capability('testcap 2', servant.Text("topkek"), servant.Text("salat"))
    def testcap2(self, text1, text2):
        raise EnvironmentError('hi')
        print('testcap2 called with {}'.format(text1))

cat = TestCat()
__module__.add(cat)

print(cat.testcap1)
print(cat.testcap2)