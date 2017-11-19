import sys
import os
import json

class Test:
    def __init__(self, appName = "dummy_app"):
        self.appName = appName

    def loadFile(self, fileName):
        try:
            self.testCases = json.load(open(fileName))
        except IOError, e:
            print("Error: %s" % str(e))
            sys.exit(0)

    def executeTest(self, test):
        return os.system("./" + self.appName + " " + test["parameters"] + " > /dev/null")

    def checkTestResult(self, test):
        expected = test["result"].lower()
        if (expected != "pass") and (expected != "fail"):
            print("Error: Test results can only be 'fail' or 'pass'")
            return True

        return (self.executeTest(test) == 0) ^ (expected == "pass")

    def run(self):
        try:
            for test in self.testCases:
                testStr = "+ Test \"" + test["name"] + "\": "
                if self.checkTestResult(test):
                    print(testStr + "FAILED")
                    return
                print(testStr + "PASSED")

        except AttributeError, attrErr:
            print("Error: %s" % str(attrErr));

        except KeyError, keyErr:
            print("Error: No Attribute %s in test Json" % str(keyErr));


def parseInputArgs(argv):
    if len(argv) > 1:
        testCasesFileName = argv[1]
    else:
        print("Error: Not enough input arguments")
        sys.exit(0)

    return testCasesFileName 

testCasesFileName = parseInputArgs(sys.argv)

test = Test("dummy_app")
test.loadFile(testCasesFileName)
test.run()
