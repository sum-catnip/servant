import servant
import inspect

def capability(name, *args):
    def decorator(func): 
        func.capability_name = name
        func.capability_args = args
        return func
    return decorator

class Category(servant.Category):
    def __init__(self, name, id_):
        # need to do this before getmembers to prevent 'MemoryError: bad allocation'
        super().__init__(name, id_)
        for item in inspect.getmembers(self):
            value = item[1]
            if hasattr(value, 'capability_name') and hasattr(value, 'capability_args'):
                self.add(servant.Capability(value.capability_name, value, list(value.capability_args)))