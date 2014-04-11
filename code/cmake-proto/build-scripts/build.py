"""
Build system driver to unify CMake and make, enabling automated re-running
of CMake when necessary
"""


import argparse
import os
import subprocess
import string


BASH_COLOR_CODES = {
	"clear":       "\033[0m",
	"bold":        "\033[1m",
	"black":       "\033[0;30m",
	"red":         "\033[0;31m",
	"green":       "\033[0;32m",
	"yellow":      "\033[0;33m",
	"blue":        "\033[0;34m",
	"purple":      "\033[0;35m",
	"turquoise":   "\033[0;36m",
	"b_black":     "\033[1;30m",
	"b_red":       "\033[1;31m",
	"b_green":     "\033[1;32m",
	"b_yellow":    "\033[1;33m",
	"b_blue":      "\033[1;34m",
	"b_purple":    "\033[1;35m",
	"b_turquoise": "\033[1;36m"
}


def colorize(format):
	return string.Formatter().vformat(format, (), BASH_COLOR_CODES) + BASH_COLOR_CODES["clear"]

def print_colors():
	for color, code in BASH_COLOR_CODES.items():
		print('   ', code, color, sep='')
	print(BASH_COLOR_CODES["clear"])



"""
	Return true if build files are out of date and CMake needs re-running
"""
def sourceset_changed():
	"""
		delegating this check to make, since the check requires project-specific
		information that originates from the CMake files and is embedded in the 
		makefiles. This avoids replicating that information here or finding some
		other place for it.
		The make target we call is a phony target that doesn't generate anything,
		but results in success if build files are up to date, failure otherwise
	"""
	return 0 != subprocess.call(["make", "up-to-date-check"])


def ask_for_sourcset_change_action():
	print("Asking")
	return "abort"


def parse_args():
	parser = argparse.ArgumentParser(description="Build system driver unifying CMake and make")
	parser.add_argument("source_root", help="The root directory of the source tree to build")

	parser.add_argument(
		"-f", "--force-meta-rebuild",
		dest='force_meta_rebuild',
		action="store_true",
		help="Forces a meta rebuild")

	parser.add_argument(
		"-n", "--no-build",
		dest='no_build',
		action="store_true",
		help="Don't perform the build; meta rebuild will still be performed if required")

	parser.add_argument("--on-sourceset-change",
		choices=["abort", "ignore", "rebuild", "ask"],
		default="abort",
		metavar='', # suppress automatic output of choices in help
		dest='on_src_change',
		help=colorize(
			"Action to take when source files have been added or removed. Allowed values are "
			"{bold}abort{clear} (stop immediately and exit with an error), {bold}ignore{clear} (continue as if there were no"
			"change in sourceset), {bold}rebuild{clear} (re-run the meta-build step to take account of new files)"
			", or {bold}ask{clear} (ask each time; requires interactive terminal)"))
	
	return parser.parse_args()


def main():
	# print_colors()
	args = parse_args()

	rebuild = False

	if not os.path.isfile("Makefile"):
		print( colorize("{bold}Makefile not found; will re-run meta build") )
		rebuild = True
	elif args.force_meta_rebuild:
		rebuild = True
	elif args.on_src_change != 'ignore' and sourceset_changed():
		if args.on_src_change == 'ask':
			args.on_src_change = ask_for_sourcset_change_action()

		if args.on_src_change == "abort":
			print( colorize("{b_red}Aborting") )
			return 1

		if args.on_src_change == 'rebuild':	
			print( colorize("{b_green}Will re-run meta build") )
			rebuild = True
	
	# create a logger process to copy logged messages to a file
	# logger = subprocess.Popen(['tee', 'build.log'], stdin=subprocess.PIPE)

	# run meta build if required
	if rebuild:
		print( colorize("{bold}Re-running meta build") )

		result = subprocess.call(['cmake', args.source_root, "-DWITH_CPPUNIT=1"])#, stdout=logger.stdin, stderr=subprocess.STDOUT)

		if ( result != 0 ):
			print( colorize("{b_red}Meta-build failed; aborting") )
			return 1

	# run build if required
	if not args.no_build:
		print( colorize("{bold}Building...") )
		result = subprocess.call(["make", "-j32"])#, stdout=logger.stdin, stderr=subprocess.STDOUT)

	


if __name__ == "__main__":
	main()
