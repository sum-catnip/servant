import servant

print('waddup pimps from testmod1')

def test():
    print('HUHU!')

# ok so why do i have the parent in the constructor but still have to explicitly add
# i should probs remove it from the constructor 

cat = servant.Category(__module__, "salat")
cap = servant.Capability(cat, "keks", test)
cat.add_capability(cap)
__module__.add_category(cat)
print(cap.name)