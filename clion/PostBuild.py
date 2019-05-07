import sys

args = sys.argv

print('Post Build Process : ' + args[1])

with open(args[1], 'a') as f:
    print('\n\nexport default Module;\n', file=f)