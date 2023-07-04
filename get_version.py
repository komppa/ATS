'''
    This script takes version number from VERSION file and
    passes it to the compiler as a definition.
'''


Import("env")

version = open('VERSION', 'r').read().strip()

env.Append(CPPDEFINES=[
    ("SW_VERSION", "\\\"{}\\\"".format(version))
])