"""
Build system driver to unify CMake and make, enabling automated re-running
of CMake when necessary
"""


import argparse
import subprocess



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

def main():
	parser = argparse.ArgumentParser(description="Build system driver unifying CMake and make")
	parser.add_argument("source_root", help="The root directory of the source tree to build")

	parser.add_argument("--on-sourceset-change",
		choices=["abort", "ignore", "rebuild", "ask"],
		default="abort",
		metavar='', # suppress automatic output of choices in help
		dest='on_src_change',
		help="Action to take when source files have been added or removed. Allowed values are "
			"abort (stop immediately and exit with an error), ignore (continue as if there were no"
			"change in sourceset), rebuild (re-run the meta-build step to take account of new files)"
			", or ask (ask each time; requires interactive terminal)")
	args = parser.parse_args()

	if args.on_src_change != 'ignore' and sourceset_changed():
		if args.on_src_change == 'ask':
			args.on_src_change = ask_for_sourcset_change_action()

		if args.on_src_change == "abort":
			print("Aborting")
			return 1

		if args.on_src_change == 'rebuild':	
			print("Re-running meta build")
			subprocess.call(["cmake", args.source_root])

	# TODO: make this NOT fail if ignoring srcset change
	subprocess.call(["make"])

	


if __name__ == "__main__":
	main()
