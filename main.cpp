#include "anco_pool.h"

int main(int argc, char* argv[]) {
	Anco_pool& anco_pool = Anco_pool::get();
	anco_pool.run();

	return true;
}

