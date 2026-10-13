/**
 * SVFIR.cpp
 * @author kisslune
 */

#include "Graphs/SVFG.h"
#include "SVF-LLVM/SVFIRBuilder.h"

using namespace SVF;
using namespace llvm;
using namespace std;

int main(int argc, char** argv)
{
    int arg_num = 0;
    int extraArgc = 4;
    char** arg_value = new char*[argc + extraArgc];
    for (; arg_num < argc; ++arg_num) {
        arg_value[arg_num] = argv[arg_num];
    }
    std::vector<std::string> moduleNameVec;

    int orgArgNum = arg_num;
    arg_value[arg_num++] = (char*)"-model-arrays=true";
    arg_value[arg_num++] = (char*)"-pre-field-sensitive=false";
    arg_value[arg_num++] = (char*)"-model-consts=true";
    arg_value[arg_num++] = (char*)"-stat=false";
    assert(arg_num == (orgArgNum + extraArgc) && "more extra arguments? Change the value of extraArgc");

    moduleNameVec = OptionBase::parseOptions(arg_num, arg_value, "SVF IR", "[options] <input-bitcode...>");

    LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);

    // Instantiate an SVFIR builder
    SVFIRBuilder builder;
    cout << "Generating SVFIR(PAG), call graph and ICFG ..." << endl;

    // TODO: here, generate SVFIR(PAG), call graph and ICFG, and dump them to files
    SVFIR* pag = builder.build();

    /// 1) SVFIR (PAG)  ->  ./svfir.dot
    pag->dump("svfir");

    /// 2) ICFG         ->  ./icfg.dot
    ICFG* icfg = pag->getICFG();
    icfg->dump("icfg");

    /// 3) Call graph   ->  ./callgraph.dot
    CallGraph* callgraph = pag->getCallGraph();
    callgraph->dump("callgraph");

    SVFIR::releaseSVFIR();
    LLVMModuleSet::releaseLLVMModuleSet();

    delete[] arg_value;  

    return 0;
}
