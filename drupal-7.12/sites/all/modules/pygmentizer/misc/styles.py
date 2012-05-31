#! /usr/bin/python

#
# Print out all Pygments styles for HTML
#

import commands
import re

styles = re.findall(r"\* (\w+):", commands.getoutput("pygmentize -L styles"))

p = re.compile("^", re.M)
for s in styles:
  css_code = re.sub(p, (".%s " % s), commands.getoutput("pygmentize -S %s -f html" % s))
  print css_code
