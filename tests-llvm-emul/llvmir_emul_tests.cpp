/**
* @file tests/llvmir-emul/llvmir_emul_tests.cpp
* @brief Tests for the @c LlvmIrEmulator class.
* @copyright (c) 2017 Avast Software, licensed under the MIT license
*/

#include "lib/llvmir-emul.h"
#include "llvmir_emul_tests.h"
#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/IRBuilder.h>

using namespace std;
using namespace llvm;

int main() {
// ========================================================
//    test0
// ========================================================
//    LLVMContext ctx;
//    SMDiagnostic err;
//
//    cout << "test begin" << endl;
//    unique_ptr<Module> m(parseIRFile("test.ll", err, ctx));
//
//    cout << "begin" << endl;
//    llvm::Function* st0 = getFunctionByName("main", m.get());
//    llvm::GlobalValue* st1 = getGlobalByName("_ZL1c", m.get());
//    st0->dump();
//    st1->dump();
//    retdec::llvmir_emul::LlvmIrEmulator emu(m.get());
//    emu.runFunction(st0);
//    string s = emu.getGlobalVariableValue(getGlobalByName("_ZL1c", m.get())).IntVal.toString(10, 0);
//    cout << s << endl;
//    cout << "Hello, World!" << endl;

// ========================================================
//    test1
// ========================================================
//    LLVMContext ctx1;
//    SMDiagnostic err1;
//
//    cout << "test begin" << endl;
//    unique_ptr<Module> m1(parseIRFile("test1.ll", err1, ctx1));
//
//    cout << "begin" << endl;
//    llvm::Function* st2 = getFunctionByName("_Z5shiftv", m1.get());
//    llvm::GlobalValue* st3 = getGlobalByName("_ZL1d", m1.get());
//    st2->dump();
//    st3->dump();
//    retdec::llvmir_emul::LlvmIrEmulator emu1(m1.get());
//    emu1.runFunction(st2);
//    string s1 = emu1.getGlobalVariableValue(getGlobalByName("_ZL1d", m1.get())).IntVal.toString(2, 0);
//    cout << s1 << endl;
//    cout << "Hello, World!" << endl;
//
// ========================================================
//    test2
// ========================================================
//    LLVMContext ctx2;
//    SMDiagnostic err2;
//
//    unique_ptr<Module> m2(parseIRFile("test1.ll", err2, ctx2));
//
//    cout << "begin" << endl;
//    llvm::Function* st4 = getFunctionByName("_Z5shiftii", m2.get());
//    llvm::GlobalValue* st5 = getGlobalByName("_ZL1d", m2.get());
//    retdec::llvmir_emul::LlvmIrEmulator emu2(m2.get());
//
//    GenericValue a[2];
//    a[0].IntVal = APInt(32, 1000);
//    a[1].IntVal = APInt(32, 99);
//    const llvm::ArrayRef<llvm::GenericValue> argVals(a, a+2);
//
//    GenericValue gv = emu2.runFunction(st4, argVals);
//    cout << gv.IntVal.toString(10, 0) << endl;
//    cout << "end finally" << endl;


// ========================================================
//    test3
// ========================================================
//    LLVMContext ctx3;
//    SMDiagnostic err3;
//
//    unique_ptr<Module> m3(parseIRFile("./wget_opt.bc", err3, ctx3));
//    retdec::llvmir_emul::LlvmIrEmulator emu3(m3.get());
//
//    cout << "begin" << endl;
//    llvm::Function* st4 = getFunctionByName("function_403f80", m3.get());
//    st4->dump();
//    llvm::GenericValue tgv = emu3.runFunction(st4);
//    cout << tgv.IntVal.toString(10,0) << endl;
//
//    llvm::Function* st5 = getFunctionByName("function_403f90", m3.get());
//    st5->dump();
////    runOnFunctions<int>(m3.get());
//    GenericValue a[3];
//    a[0].IntVal = APInt(64, 1000);
//    a[1].IntVal = APInt(64, 99);
//    a[2].IntVal = APInt(64, 990);
//    const llvm::ArrayRef<llvm::GenericValue> argVals(a);
//    llvm::GenericValue tgv1 = emu3.runFunction(st5, argVals);
//    cout << tgv1.IntVal.toString(10,0) << endl;

// ========================================================
//    test4
// ========================================================

// ----
//    LLVMContext ctx4;
//    SMDiagnostic err4;
//
//    unique_ptr<Module> m4(parseIRFile("./x64_gzip_1_10_gcc_O2.ll", err4, ctx4));
//    retdec::llvmir_emul::LlvmIrEmulator emu4(m4.get());
//
//    cout << "begin" << endl;
// ----

//    llvm::Function* st0 = getFunctionByName("_init", m4.get());
//    st0->dump();
//    GenericValue p;
//    p.IntVal = APInt(64, 1000);
//    const llvm::ArrayRef<llvm::GenericValue> argVals(p);
//    llvm::GenericValue tgv = emu4.runFunction(st0, p);
//    cout << tgv.IntVal.toString(10,0) << endl;
//
//    llvm::Function* st1 = getFunctionByName("function_1f90", m4.get());
//    llvm::GenericValue tgv1 = emu4.runFunction(st1);
//    cout << tgv1.IntVal.toString(10,0) << endl;

// ----
//    llvm::Function* st2 = getFunctionByName("function_21d6", m4.get());
////    GenericValue p2;
////    char *array2 = "icse";
////    p2.PointerVal = array2;
//    llvm::GenericValue tgv2 = emu4.runFunction(st2);
//    cout << tgv2.IntVal.toString(10,0) << endl;
// ----

// ========================================================
//    test5
// ========================================================
//
//    LLVMContext ctx5;
//    SMDiagnostic err5;
//
//    unique_ptr<Module> m5(parseIRFile("./x64_gzip_1_10_gcc_O2.ll", err5, ctx5));
//    retdec::llvmir_emul::LlvmIrEmulator emu5(m5.get());
//
//    cout << "begin" << endl;
//    GenericValue a[2];
//
//    llvm::Function* st2 = getFunctionByName("function_21e0", m5.get());
//    for(auto i = st2->arg_begin() ; i != st2->arg_end() ; i++ )
//    {
//        int type = i->getType()->getTypeID();
//        cout<< i->getType()->getTypeID() << endl;
//        cout << i->getType()->getIntegerBitWidth() << endl;
//        if(type == 10)
//        {
//            cout << "test00" << endl;
//            a[0].IntVal = APInt(10, 99);
//        }
//        else if(type == 14)
//        {
//            cout << "test01" << endl;
//            a[1].PointerVal = reinterpret_cast<PointerTy>(0x2323A24B);
//            cout << a[1].PointerVal << endl;
//        }
//    }
//    llvm::GenericValue tgv2 = emu5.runFunction(st2, a);
//    cout << tgv2.IntVal.toString(10,0) << endl;
//    runFunctions("./x64_gzip_1_10_gcc_O2.ll");


    map<string, string> functionMap1, functionMap2;
    irMap.emplace("./x64_wget_1_20_3_gcc75_O0.ll", functionMap1);
    irMap.emplace("./x64_wget_1_20_3_gcc75_O3.ll", functionMap2);


//
//    runFunctionWithParameters("./x64_gzip_1_10_gcc_O2.ll", "bi_windup");
//    runFunctionWithParameters("./x64_gzip_1_10_gcc75_O0_3.6.2.ll", "bi_windup");
//    runFunctionWithParameters("./x64_gzip_1_10_gcc75_O0_3.6.2.ll", "send_all_trees");
//    runFunctionWithParameters("./x64_gzip_1_10_gcc_O2.ll", "send_bits");
//    runFunctionWithParameters("./x64_gzip_1_10_gcc75_O0_3.6.2.ll", "send_bits");

    cout << "begin" << endl;
    runFunctions("./x64_wget_1_20_3_gcc75_O0.ll");
    cout << "interval" << endl;
    runFunctions("./x64_wget_1_20_3_gcc75_O3.ll");

//    ifstream infile1;
//    infile1.open("./gzipO0.txt");
//    string first;
//    if(infile1.is_open()){
//        while(getline(infile1, first)) {
//            int pos = first.find(" ");
//            functionMap1.emplace(first.substr(0, pos), first.substr(pos+1, first.size()));
//        }
//    }
//    infile1.close();
//    irMap.emplace("./gzipO0.txt", functionMap1);
//
//    ifstream infile2;
//    infile2.open("./gzipO2.txt");
//    string second;
//    if(infile2.is_open()){
//        while(getline(infile2, second)) {
//            int pos = second.find(" ");
//            functionMap2.emplace(second.substr(0, pos), second.substr(pos+1, second.size()));
//        }
//    }
//    infile2.close();
//    irMap.emplace("./gzipO2.txt", functionMap2);

    ofstream outfile;
    outfile.open("tmp_output2.txt");
    for(auto i = irMap.begin(); i != irMap.end() ; i++){
        int right = 0, count = 0;
//        cout << i->first << " is the IR file" << endl;
//        cout << endl;
        outfile << i->first << " is the IR file" << endl;
        outfile << endl;
        for(auto j = i->second.begin() ; j != i->second.end() ; j++){
//            cout << j->first << " is the function name" << endl;
//            cout << endl;
            outfile << j->first << " is the function name" << endl;
            outfile << endl;
            double max = 0;
            string initial = "null";
            string s1 = j->second;
            string sTmp = "";
            string s2 = "";
            for(auto m = irMap.begin(); m != irMap.end() ; m++){
                if(m != i){
                    for(auto n = m->second.begin(); n != m->second.end() ; n++){
                        sTmp = n->second;
                        double tmp = JaccardSimilarity(s1, sTmp);
                        if(tmp == max && n->first == j->first){
                            max = tmp;
                            initial = n->first;
                            s2 = sTmp;
                        }
                        else if(tmp > max){
                            max = tmp;
                            initial = n->first;
                            s2 = sTmp;
                        }
                    }
                }
            }
//            cout << "the corresponding function in another IR file is " << initial << endl;
//            cout << "the corresponding similarity is " << max << endl;
//            cout << "the first similarity string is " << s1 << endl;
//            cout << "the second similarity string is " << s2 << endl;
            outfile << "the corresponding function in another IR file is " << initial << endl;
            outfile << "the corresponding similarity is " << max << endl;
            outfile << "the first similarity string is " << s1 << endl;
            outfile << "the second similarity string is " << s2 << endl;
//            cout << "================" << endl;
            outfile << "================" << endl;
            count ++ ;
            if(initial == j->first) right ++;
//            else {
//                cout << j->first << " " << initial << " " << max << endl;
//                cout << s1 << endl << s2 << endl;
//            }
        }
        cout << "====================finish=========================" << endl;
        outfile << "=====================================================" << endl;
        cout << "The precision is " << float(right * 100 /count) << "%" <<endl;
        cout << right << " " << count << endl;
    }
    outfile.close();
    return 0;
}