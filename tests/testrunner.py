import glob
import os


def main():
    tests = 0
    failed = 0
    for test in glob.glob('bin/tests/*'):
        print ''
        print "=== TESTING", test, " ==="
        if os.system(test):
            failed += 1
        tests += 1

    print ''
    print "====================================="
    print "====================================="
    if failed:
        print "\033[;31m"
    else:
        print "\033[;32m"
    print "Ran", tests, "test suite" + ("s" if tests > 1 else "") + ",",
    print failed, "failed"
    print '\033[0m'

main()
