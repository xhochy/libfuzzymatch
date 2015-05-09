#include <iostream>

#include "common/args.h"
#include "common/plain_index.h"
#include "common/timer.h"

void usage(char** argv) {
	std::cout << "Usage: " << argv[0] << " [options]" << std::endl << std::endl
		<< "-a <algo>    -- algorithm for which to build an index. Choices: {pairwise, static, limit}, lucene++" << std::endl
		<< "-i <infile>  -- input filename for index construction" << std::endl
		<< "-o <outfile> -- output filename for index" << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	ArgParser args(argc, argv);

	if (!args.isSet("i")) usage(argv);

	const std::string algo = args.get<std::string>("a", "pairwise"),
		infile  = args.get<std::string>("i"),
		outfile = args.get<std::string>("o", "index.plain");

	Timer timer;
	if (algo == "pairwise" || algo == "static" || algo == "limit") {
		indexFromFile(infile.c_str(), outfile.c_str());
	} else if (algo == "lucene++") {
		// TODO xhochy
	} else {
		usage(argv);
	}

	std::cout << "Built index in " << timer.get() << "s" << std::endl;
}
