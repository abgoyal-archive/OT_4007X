import test
import os
from os.path import join, dirname, exists, basename, isdir
import re

FLAGS_PATTERN = re.compile(r"//\s+Flags:(.*)")

class MessageTestCase(test.TestCase):

  def __init__(self, path, file, expected, mode, context, config):
    super(MessageTestCase, self).__init__(context, path)
    self.file = file
    self.expected = expected
    self.config = config
    self.mode = mode

  def IgnoreLine(self, str):
    """Ignore empty lines and valgrind output."""
    if not str: return True
    else: return str.startswith('==') or str.startswith('**')

  def IsFailureOutput(self, output):
    f = file(self.expected)
    # Skip initial '#' comment and spaces
    for line in f:
      if (not line.startswith('#')) and (not line.strip()):
        break
    # Convert output lines to regexps that we can match
    env = { 'basename': basename(self.file) }
    patterns = [ ]
    for line in f:
      if not line.strip():
        continue
      pattern = re.escape(line.rstrip() % env)
      pattern = pattern.replace('\\*', '.*')
      pattern = '^%s$' % pattern
      patterns.append(pattern)
    # Compare actual output with the expected
    raw_lines = output.stdout.split('\n')
    outlines = [ s for s in raw_lines if not self.IgnoreLine(s) ]
    if len(outlines) != len(patterns):
      return True
    for i in xrange(len(patterns)):
      if not re.match(patterns[i], outlines[i]):
        return True
    return False

  def GetLabel(self):
    return "%s %s" % (self.mode, self.GetName())

  def GetName(self):
    return self.path[-1]

  def GetCommand(self):
    result = [self.config.context.GetVm(self.mode)]
    source = open(self.file).read()
    flags_match = FLAGS_PATTERN.search(source)
    if flags_match:
      result += flags_match.group(1).strip().split()
    result.append(self.file)
    return result

  def GetSource(self):
    return (open(self.file).read()
          + "\n--- expected output ---\n"
          + open(self.expected).read())


class MessageTestConfiguration(test.TestConfiguration):

  def __init__(self, context, root):
    super(MessageTestConfiguration, self).__init__(context, root)

  def Ls(self, path):
    if isdir(path):
        return [f[:-3] for f in os.listdir(path) if f.endswith('.js')]
    else:
        return []

  def ListTests(self, current_path, path, mode):
    mjsunit = [current_path + [t] for t in self.Ls(self.root)]
    regress = [current_path + ['regress', t] for t in self.Ls(join(self.root, 'regress'))]
    bugs = [current_path + ['bugs', t] for t in self.Ls(join(self.root, 'bugs'))]
    all_tests = mjsunit + regress + bugs
    result = []
    for test in all_tests:
      if self.Contains(path, test):
        file_prefix = join(self.root, reduce(join, test[1:], ""))
        file_path = file_prefix + ".js"
        output_path = file_prefix + ".out"
        if not exists(output_path):
          print "Could not find %s" % output_path
          continue
        result.append(MessageTestCase(test, file_path, output_path, mode,
                                      self.context, self))
    return result

  def GetBuildRequirements(self):
    return ['sample', 'sample=shell']

  def GetTestStatus(self, sections, defs):
    status_file = join(self.root, 'message.status')
    if exists(status_file):
      test.ReadConfigurationInto(status_file, sections, defs)


def GetConfiguration(context, root):
  return MessageTestConfiguration(context, root)
