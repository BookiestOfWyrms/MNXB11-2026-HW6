{
gROOT->LoadMacro("MovingObject.cxx+");
gROOT->LoadMacro("write.cxx");
std::cout << "write.cxx has been loaded."
              << std::endl;

gROOT->LoadMacro("read.cxx");
std::cout << "read.cxx has also been loaded."
              << std::endl;
}