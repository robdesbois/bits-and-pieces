/* Snippets taken from the following blog post:
 * http://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
 */
#include <cerrno>
#include <fstream>
#include <iostream>
#include <string>

int main()
{
	std::string line;
	std::ifstream f("file");
	if (!f.is_open())
		perror("error while opening file");

	/* getline() returns !f.fail(), so is true if the line was read ok,
	 * *including* if EOF was encountered before EOL, and false otherwise.
	 * This means if the last line has no terminating EOL, the *following*
	 * loop iteration's call to getline() will return false, since reading
	 * from an istream with eofbit set *is* invalid.
	 */
	while(std::getline(f, line))
		std::cout << "\"" << line << "\"" << std::endl;

	if (f.bad())
		perror("error while reading file");
}

