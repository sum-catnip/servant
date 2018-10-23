import servant

print('waddup pimps from testmod1')

def test():
    print('HUHU!')

# ok so why do i have the parent in the constructor but still have to explicitly add
# i should probs remove it from the constructor 

cat = servant.Category("salat", [servant.Capability("keks", test, [servant.Text()])])
__module__.add(cat)