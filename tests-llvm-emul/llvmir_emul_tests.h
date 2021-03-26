/**
 * @file tests/llvm-emul/llvmir_tests.h
 * @brief A base test class for all tests which works with LLVM IR strings.
 * @copyright (c) 2017 Avast Software, licensed under the MIT license
 */

#ifndef LLVML_EMUL_LLVMIR_TESTS_H
#define LLVML_EMUL_LLVMIR_TESTS_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "utils/string.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/LoopInfoImpl.h>
#include <llvm/Analysis/DomPrinter.h>
#include <time.h>
using namespace std;

static map<string, map<string, string>> irMap;
//static map<string, string> functionMap;
/**
 * Utility function to get value (i.e. global variable, function,
 * instruction) with the specified @c n from module @c m.
 * Values are searched in the following order:
 *   1. globals
 *   2. functions
 *   3. instructions
 * The first match is returned, therefore it is best to make sure
 * all values in module have unique names.
 *
 * Typical usage:
 *   1. create module using @c parseInput()
 *   2. if you need some specific values to work with, get them using
 *      this function.
 * This is preferable to:
 *   1. create module using @c addModuleToContext()
 *   2. manually create objects and add them to module
 * because manual object creation is not pretty -- it is very hard to
 * see what the resulting module looks like.
 */
llvm::Value* getValueByName(const std::string& n, llvm::Module *module)
{
    for (auto& g : module->getGlobalList())
    {
        if (g.getName() == n)
        {
            return &g;
        }
    }
    for (auto& f : module->getFunctionList())
    {
        if (f.getName() == n)
        {
            return &f;
        }
    }
    for (auto& f : module->getFunctionList())
    {
        for (auto &b : f)
            for (auto &i : b)
            {
                if (i.getName() == n)
                {
                    return &i;
                }
            }
    }
    throw std::runtime_error("failed to find specified value: " + n);
    return nullptr;
}

/**
 * Print any LLVM object which implements @c print(llvm::raw_string_ostream&)
 * method into std::string.
 * @param t LLVM object to print.
 * @return String with printed object.
 *
 * TODO: This is on multiple places in this repo.
 */
template<typename T>
std::string llvmObjToString(const T* t)
{
    std::string str;
    llvm::raw_string_ostream ss(str);
    if (t)
        t->print(ss);
    else
        ss << "nullptr";
    return ss.str();
}
std::string llvmObjToString(const llvm::Module* t)
{
    std::string str;
    llvm::raw_string_ostream ss(str);
    if (t)
        t->print(ss, nullptr);
    else
        ss << "nullptr";
    return ss.str();
}

int num_common_tokens(vector <string> &tokens1, vector <string> &tokens2)
{
    sort(tokens1.begin(), tokens1.end());
    sort(tokens2.begin(), tokens2.end());
    int matches = 0;
    for(int i = 0, j = 0; i < tokens1.size() && j < tokens2.size();)
    {
        if(tokens1[i] == tokens2[j])
        {
            matches++;
            i++;
            j++;
        }
        else if(tokens1[i] < tokens2[j])
        {
            i++;
        }
        else j++;
    }
    return matches;
}

//Tokenizes the string given the delimiters
int Tokenize_with_delimiter(const string &str,vector <string> * sentences, const string delimiters)
{
    int num_words = 0;
    string::size_type lastPos = str.find_first_not_of(delimiters);
    string::size_type pos  = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        sentences->push_back(str.substr(lastPos,pos-lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
        num_words++;

    }
    return sentences->size();;
}

/**
 * Print LLVM diagnostics error.
 * @param err LLVM diagnostic error.
 */
void printLLVMIRConversionError(const llvm::SMDiagnostic& err)
{
    err.print("", llvm::errs());
}

/**
 * Parse the provided LLVM IR @c code into a LLVM module.
 * @param code LLVM IR code string.
 * @param ctx LLVM IR context.
 * @return LLVM module created by parsing the provided @c code.
 */
std::unique_ptr<llvm::Module> _parseInput(
        const std::string& code,
        llvm::LLVMContext& ctx)
{
    std::string c =
            "target datalayout = \"e-p:32:32:32-f80:32:32\"\n"
            + code;
    std::unique_ptr<llvm::MemoryBuffer> mb(llvm::MemoryBuffer::getMemBuffer(llvm::StringRef(c)));
    if (mb == nullptr) {
        throw std::runtime_error("failed to create llvm::MemoryBuffer");
    }
    std::cout << mb->getMemBufferRef().getBuffer().str() << std::endl;
    std::cout << mb->getMemBufferRef().getBufferIdentifier().str() + " info" << std::endl;
    llvm::SMDiagnostic err;
    std::unique_ptr<llvm::Module> m(parseIR(mb->getMemBufferRef(), err, ctx));
    if (m == nullptr) {
        printLLVMIRConversionError(err);
        throw std::runtime_error("invalid LLVM IR");
    }
    std::cout << "_parseInput end" << std::endl;
    return m;
}

/**
     * Utility function to get LLVM function.
     * This only casts @c getValueByName() result to @c llvm::Function.
     */
llvm::Function* getFunctionByName(const std::string& n, llvm::Module *module)
{
    auto* v = getValueByName(n, module);
    return llvm::dyn_cast_or_null<llvm::Function>(v);
}

/**
     * Utility function to get LLVM global variable.
     * This only casts @c getValueByName() result to @c llvm::GlobalVariable.
     */
llvm::GlobalVariable* getGlobalByName(const std::string& n, llvm::Module *module)
{
    auto* v = getValueByName(n, module);
    return llvm::dyn_cast_or_null<llvm::GlobalVariable>(v);
}

///**
// * Check if the IR string from @c actual LLVM module is the same
// * as @c expected
// * LLVM IR string.
// * @param expected LLVM IR string.
// */
//void checkModuleAgainstExpectedIr(std::string& expected, llvm::Module *module)
//{
//    llvm::LLVMContext expectedContext;
//    auto expectedModule = _parseInput(expected, expectedContext);
//
//    std::string expectedStr = llvmObjToString(expectedModule.get());
//    std::string actualStr = llvmObjToString(module);
//
//    if(!llvm::verifyModule(*expectedModule))
//        std::cout << "expected module is not valid:\n" << expectedStr;
//    if(verifyModule(*module))
//        std::cout<< "actual module is not valid:\n" << actualStr;
//
//    expectedStr = retdec::utils::removeComments(expectedStr, ';');
//    actualStr = retdec::utils::removeComments(actualStr, ';');
//
//    if(
//            retdec::utils::removeWhitespace(expectedStr) ==
//            retdec::utils::removeWhitespace(actualStr)
//            )
//        std::cout << "expected:\n"
//                  << "=========\n" << expectedStr << "\n"
//                  << "actual:\n"
//                  << "=======\n" << actualStr << "\n";
//}


/**
 * Utility function to get LLVM instruction.
 * This only casts @c getValueByName() result to @c llvm::Instruction.
 */
llvm::Instruction* getInstructionByName(const std::string& n, llvm::Module *module)
{
    auto* v = getValueByName(n, module);
    return llvm::dyn_cast_or_null<llvm::Instruction>(v);
}

/**
 * Utility function to get @c n Nth (default zero = first) instruction
 * of specified type from module @a m.
 */
template<typename T>
T* getNthInstruction(llvm::Module* module, unsigned n = 0)
{
    unsigned cntr = 0;
    for (auto& f : *module)
        for (auto& b : f)
            for (auto& i : b)
            {
                if (auto* r = llvm::dyn_cast<T>(&i))
                {
                    if (cntr == n)
                    {
                        return r;
                    }
                    else
                    {
                        ++cntr;
                    }
                }
            }

    return nullptr;
}

/**
 * Utility function to get @c n Nth (default zero = first) instruction
 * of specified type from function @a f.
 */
template<typename T>
T* getNthInstruction(llvm::Function* f, unsigned n = 0)
{
    unsigned cntr = 0;
    for (auto& b : *f)
        for (auto& i : b)
        {
            if (auto* r = llvm::dyn_cast<T>(&i))
            {
                if (cntr == n)
                {
                    return r;
                }
                else
                {
                    ++cntr;
                }
            }
        }

    return nullptr;
}

// TODO: Variadic templates to accept and pass any arguments.
template<typename T>
void runOnFunctionCustom(T& pass, llvm::Module* m)
{
    pass.doInitialization(*m);

    for (auto& f : m->functions())
    {
        pass.runOnFunctionCustom(f);

    }

    pass.doFinalization(*m);
}

// TODO: Variadic templates to accept and pass any arguments.
// TODO: Use in all existing tests.
template<typename T>
void runOnModuleCustom(T& pass, llvm::Module* m)
{
    pass.doInitialization(*m);
    pass.runOnModuleCustom(*m);
    pass.doFinalization(*m);
}

void initializeLlvmPassRegistry()
{
    // See llvm/tools/opt -- There are a lot more initializations,
    // but we use only those we need here.
    llvm::PassRegistry& registry = *llvm::PassRegistry::getPassRegistry();
    llvm::initializeAnalysis(registry);
    llvm::initializeTransformUtils(registry);
}

template<typename T>
void runOnModule(llvm::Module* module)
{
    initializeLlvmPassRegistry();

    llvm::legacy::PassManager passManager;
    passManager.add(new T());
    passManager.run(*module);
}

template<typename T>
void runOnFunctions(llvm::Module *module)
{
    initializeLlvmPassRegistry();

    llvm::legacy::FunctionPassManager passManager(module);
    passManager.add(new T());

    passManager.doInitialization();
    for (llvm::Function& f : module->functions())
    {
        passManager.run(f);
    }
    passManager.doFinalization();
}

char* readFromFile(string str, int length, int bytes)
{
//    cout << "length - " << length << " bytes - " << bytes << endl;
    char c;
    int size = length/4;
    if(4 * size < length) size += 2;
    char array_c[size];
    ifstream infile;
    infile.open(str);
    infile.seekg(bytes);
    if(!infile.is_open()){
//        cout << "the file could not be opened!" << endl;
        // ****
        return "";
    }

    int i = 0;
    while(!infile.eof())
    {
        if(infile.eof()){
//            cout << "reinitialize" << endl;
            infile.seekg(0);
        }
        infile>>c;
//        cout << c << endl;
        array_c[i] = c;
        i++;
//        cout << i << endl;
        if(i%2==0)
        {
            infile>>c;
//            cout << "space" << endl;
            if(i==size)
            {
                infile.close();
                array_c[i]= '\0';
                return array_c;
            }
        }
    }
}

llvm::GenericValue getValue(int type, int length, int bytes)
{
    llvm::GenericValue gv;
//    cout << length << " test " << bytes << endl;
    string str = "./1611224159_random.txt";
    int i = 0;
    llvm::GenericValue result;
    string tmp = readFromFile(str, length, bytes);
    char *numTmp = new char[tmp.size()];
    llvm::StringRef strRef = tmp;
    int size = length/8;
    if(8 * size < length) size += 2;
    switch(type){
        case 0:
            break;
        case 1:
            tmp = "0X" + tmp.substr(0, tmp.size() - 5) + "p11";
            result.FloatVal=llvm::APFloat(llvm::APFloat::IEEEhalf, tmp).convertToFloat();
            break;
        case 2:
            tmp = "0X" + tmp.substr(0, tmp.size() - 5) + "p11";
            result.FloatVal=llvm::APFloat(llvm::APFloat::IEEEsingle, tmp).convertToFloat();
            break;
        case 3:
            tmp = "0X" + tmp.substr(0, tmp.size() - 5) + "p11";
            result.FloatVal=llvm::APFloat(llvm::APFloat::IEEEdouble, tmp).convertToDouble();
            break;
        case 4:
            tmp = "0X" + tmp.substr(0, tmp.size() - 5) + "p11";
            result.FloatVal=llvm::APFloat(llvm::APFloat::x87DoubleExtended, tmp).convertToDouble();
            break;
        case 5:
            tmp = "0X" + tmp.substr(0, tmp.size() - 5) + "p11";
            result.FloatVal=llvm::APFloat(llvm::APFloat::IEEEquad, tmp).convertToDouble();
            break;
        case 6:
            tmp = "0X" + tmp.substr(0, tmp.size() - 5) + "p11";
            result.FloatVal=llvm::APFloat(llvm::APFloat::PPCDoubleDouble, tmp).convertToDouble();
            break;
        case 7:
        case 8:
        case 9:
            break;
        case 10:
//            cout << tmp << endl;
//            cout << strRef.str() << endl;
            result.IntVal = llvm::APInt(length, strRef, 16);
//            result.IntVal.dump();
            break;
        case 11:
        case 12:
        case 13:
            break;
        case 14:
            tmp = "0X" + tmp;
            for(int i=0; i<tmp.size(); i++ )
            {
                numTmp[i] = tmp[i];
            }
            result.PointerVal = reinterpret_cast<llvm::PointerTy>(strtol(numTmp, NULL, 16));
//            cout << result.PointerVal << endl;
            break;
        case 15:
            break;
    }
    return result;
}

llvm::GenericValue runFunctionWithParameters(string IRFile, string functionName)
{
    llvm::LLVMContext ctx;
    llvm::SMDiagnostic err;

    unique_ptr<llvm::Module> m(parseIRFile(IRFile, err, ctx));
    llvm::DataLayout dataLayout = *m->getDataLayout();

    retdec::llvmir_emul::LlvmIrEmulator emu(m.get());

//    cout << "begin" << endl;
    llvm::Function* function = getFunctionByName(functionName, m.get());
    int size = function->arg_size();
    llvm::GenericValue *a = new llvm::GenericValue[size];
    int index = 0;
    int bytes = 0;
    string final;
//    llvm::DominatorTree DT = llvm::DominatorTree();
//    DT.recalculate(*function);
//    llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>* LoopInfo = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
//    LoopInfo->Analyze(DT);
//    string os;
//    llvm::raw_string_ostream *OS = new llvm::raw_string_ostream(os);
//    LoopInfo->print(*OS);
//    cout << OS->str() << endl;
//    cout << "loop info" << endl;
    for(auto i = function->arg_begin() ; i != function->arg_end() ; i++)
    {
//        i->dump();
        int type = i->getType()->getTypeID();
//        cout<< i->getType()->getTypeID() << endl;
//        int length = i->getType()->getIntegerBitWidth();
        int length = dataLayout.getTypeSizeInBits(i->getType());
//        cout << "length:" << length << endl;
        if(length%8==0){
            bytes += length/4 * 3;
        }
        else{
            bytes += (length/4 + 1) * 3;
        }
        a[index] = getValue(type, length, bytes);
        final.append(a[index].IntVal.toString(10, 0) + ";");
//        a[index].IntVal.dump();
        index++;
    }
    if(index == 0)
        final.append("null;");
    llvm::ArrayRef<llvm::GenericValue> argVals(a, size);
    llvm::GenericValue tgv;
    if(size > 0) {
//        cout << "call begins" << endl;
        tgv = emu.runFunction(function, argVals, true);
//        functionMap.emplace(functionName, emu.similairtyString());
//        s.emplace_back(emu.similairtyString());
    }
    else {
        tgv = emu.runFunction(function, argVals, true);
//        functionMap.emplace(functionName, emu.similairtyString());
//        s.emplace_back(emu.similairtyString());
    }
//    cout << "tests end" << endl;
//    tgv.IntVal.dump();
//    cout << tgv.IntVal.toString(10, 0) << endl;
    map<string, string> functionMap;
    functionMap = irMap.at(IRFile);
//    cout <<emu.similairtyString() << endl;
    final += emu.similairtyString() + tgv.IntVal.toString(10, 0) + ";";
//    cout << function->getName().str() << " " << final << endl;
    functionMap.emplace(functionName, final);
    irMap.erase(IRFile);
    irMap.emplace(IRFile, functionMap);
    return tgv;
}

double JaccardSimilarity(string &s1, string &s2)
{
    vector <string> tokens1;
    vector <string> tokens2;
    Tokenize_with_delimiter(s1, &tokens1, " ?\t\".,;!\n");
    Tokenize_with_delimiter(s2, &tokens2, " ?\t\".,;!\n");
    int common_tokens = num_common_tokens(tokens1,tokens2);
    int total_distinct_tokens = tokens1.size() + tokens2.size() - common_tokens;
    double similarity = (double)common_tokens/total_distinct_tokens;
    return similarity;
}

void runFunctions(string IRFile)
{
    llvm::LLVMContext ctx;
    llvm::SMDiagnostic err;

    map<string, string> functionMap;
//    irMap.erase(IRFile);
//    irMap.emplace(IRFile, functionMap);

    unique_ptr<llvm::Module> m(parseIRFile(IRFile, err, ctx));
    llvm::DataLayout dataLayout = *m->getDataLayout();

//    ofstream outfile0;
//    string file = IRFile.substr(0, IRFile.size()-3).append(".txt");
//    outfile0.open(file);

    retdec::llvmir_emul::LlvmIrEmulator emu(m.get());
//    emu.set
    int bytes = 0;
//    int flag = 0;
    for (llvm::Module::iterator function = m->begin(); function != m->end(); ++function) {
        emu.setSimilarityStringToNull();
        if(function->isDeclaration()) continue; //  ||
        if(function->getName().startswith("function_")) continue;
//        function->viewCFG();
//        if(function->getName().startswith("_")) continue;
        string final;
//        cout << "function name is " << function->getName().str() << endl;
//        if(function->getName().str() == "copy_block" || function->getName().str() == "longest_match"
//        || function->getName().str() == "rsync_roll" || function->getName().str() == "deflate_fast"
//        || function->getName().str() == "deflate") continue; // deflate_fast
//        if(function->getName().str() == "treat_stdin") continue;
//        if(function->getName().str() == "two_way_short_needle")
//            flag=1;
//        if(flag == 0) continue;
//        llvm::DominatorTree DT = llvm::DominatorTree();
//        DT.recalculate(*function);
//        llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>* LoopInfo = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
//        LoopInfo->releaseMemory();
//        LoopInfo->Analyze(DT);
//        llvm::outs() << "loop ends" << "\n";
//        cout << "loop ends" << endl;
        int size = function->arg_size();
        llvm::GenericValue *a = new llvm::GenericValue[size];
        int index = 0;

        for(auto i = function->arg_begin() ; i != function->arg_end() ; i++)
        {
//            i->dump();
            int type = i->getType()->getTypeID();
//            cout<< i->getType()->getTypeID() << endl;
//        int length = i->getType()->getIntegerBitWidth();
            int length = dataLayout.getTypeSizeInBits(i->getType());
//            cout << "length:" << length << endl;
            if(length%8==0){
                bytes += length/4 * 3;
            }
            else{
                bytes += (length/4 + 1) * 3;
            }
//            cout << "bytes" << bytes << endl;
            a[index] = getValue(type, length, bytes);
//            a[index].IntVal.dump();
            string str1 = a[index].IntVal.toString(10, 0) + ";";
            final.append(str1);
            index++;
        }
        if(index == 0)
            final.append("null;");
        llvm::ArrayRef<llvm::GenericValue> argVals(a, size);
        llvm::GenericValue tgv;
        if(size > 0) {
            tgv = emu.runFunction(function, argVals, true);
        }
        else {
            tgv = emu.runFunction(function, argVals, true);
        }
//        cout << "tests end" << endl;
//        tgv.IntVal.dump();
//        cout << tgv.IntVal.toString(10, 0) << endl;
        string str2;
//        if(tgv.IntVal.toString(10, 0) != "0")
//            str2 += emu.similairtyString() + "exist;";
//        else
        str2 = emu.similairtyString() + tgv.IntVal.toString(10, 0) + ";";
        final.append(str2);
//        cout << function->getName().str() << " " << final << endl;
        functionMap.emplace(function->getName().str(), final);
//        cout << "function ends" << endl;
//        irMap.erase(IRFile);
//        irMap.emplace(IRFile, functionMap);
    }
    irMap.erase(IRFile);
    irMap.emplace(IRFile, functionMap);
//    cout << "runFunctions end" << endl;
//    outfile0.close();
}

#endif