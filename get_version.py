'''
    This script takes version number from VERSION file and
    passes it to the compiler as a definition.
'''


Import("env")

version = open('VERSION', 'r').read().strip()

# For future (eeprom migration) purposes, here is the version number
# in hash format. 5 chars.
version_hash = str(abs(hash(version)))[:5]

env.Append(CPPDEFINES=[
    ("SW_VERSION", "\\\"{}\\\"".format(version))
])