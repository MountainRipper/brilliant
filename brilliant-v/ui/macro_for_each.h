#ifndef MACRO_FOR_EACH_IMPL
#define MACRO_FOR_EACH_IMPL
 
// clang-format off
#define ARG_OP_0(...)
#define ARG_OP_1(op, sep, arg     )  op(arg)
#define ARG_OP_2(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_1(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_3(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_2(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_4(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_3(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_5(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_4(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_6(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_5(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_7(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_6(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_8(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_7(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_9(op, sep, arg, ...)  op(arg) sep  EXPAND_(ARG_OP_8(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_10(op, sep, arg, ...) op(arg) sep  EXPAND_(ARG_OP_9(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_11(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_10(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_12(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_11(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_13(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_12(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_14(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_13(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_15(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_14(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_16(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_15(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_16(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_15(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_17(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_16(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_18(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_17(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_19(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_18(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_20(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_19(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_21(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_20(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_22(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_21(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_23(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_22(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_24(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_23(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_25(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_24(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_26(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_25(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_27(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_26(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_28(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_27(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_29(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_28(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_30(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_29(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_31(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_30(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_32(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_31(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_33(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_32(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_34(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_33(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_35(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_34(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_36(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_35(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_37(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_36(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_38(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_37(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_39(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_38(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_40(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_39(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_41(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_40(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_42(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_41(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_43(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_42(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_44(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_43(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_45(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_44(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_46(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_45(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_47(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_46(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_48(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_47(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_49(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_48(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_50(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_49(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_51(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_50(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_52(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_51(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_53(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_52(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_54(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_53(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_55(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_54(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_56(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_55(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_57(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_56(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_58(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_57(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_59(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_58(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_60(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_59(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_61(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_60(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_62(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_61(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_63(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_62(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_64(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_63(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_65(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_64(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_66(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_65(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_67(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_66(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_68(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_67(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_69(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_68(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_70(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_69(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_71(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_70(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_72(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_71(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_73(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_72(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_74(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_73(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_75(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_74(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_76(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_75(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_77(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_76(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_78(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_77(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_79(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_78(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_80(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_79(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_81(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_80(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_82(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_81(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_83(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_82(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_84(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_83(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_85(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_84(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_86(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_85(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_87(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_86(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_88(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_87(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_89(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_88(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_90(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_89(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_91(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_90(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_92(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_91(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_93(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_92(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_94(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_93(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_95(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_94(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_96(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_95(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_97(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_96(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_98(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_97(op, EXPAND_(sep), ##__VA_ARGS__))
#define ARG_OP_99(op, sep, arg, ...) op(arg) sep EXPAND_(ARG_OP_98(op, EXPAND_(sep), ##__VA_ARGS__))
 
#define ARG_SEQ() \
         99,98,97,96,95,94,93,92,91,90, \
         89,88,87,86,85,84,83,82,81,80, \
         79,78,77,76,75,74,73,72,71,70, \
         69,68,67,66,65,64,63,62,61,60, \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
          9, 8, 7, 6, 5, 4, 3, 2, 1, 0
 
#define ARG_N( \
         _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
        _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
        _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
        _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
        _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
        _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
        _61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
        _71,_72,_73,_74,_75,_76,_77,_78,_79,_80, \
        _81,_82,_83,_84,_85,_86,_87,_88,_89,_90, \
        _91,_92,_93,_94,_95,_96,_97,_98,_99, N, ...) N
 
#define MACRO_CONCAT_(A, B) A##B
#define MACRO_CONCAT(A, B) MACRO_CONCAT_(A, B)
#define EXPAND_(...) __VA_ARGS__
 
#define ARG_COUNT_(...) EXPAND_(ARG_N(__VA_ARGS__))
#define ARG_COUNT_TEST(...) EXPAND_(ARG_COUNT_(__VA_ARGS__, ARG_SEQ()))
 
#if ARG_COUNT_TEST(1,1) != 2
#error "ARG_COUNT_TEST(1,1) != 2"
#endif
#if ARG_COUNT_TEST(1) != 1
#error "ARG_COUNT_TEST(1) != 1"
#endif
 
#if ARG_COUNT_TEST() == 0 
#define ARG_COUNT(...) ARG_COUNT_(__VA_ARGS__, ARG_SEQ())
#else
#define MACRO_END_OF_ARGUMENTS_() 0
#define MACRO_PROBE() ~,1
#define MACRO_NOT_0 MACRO_PROBE()
#define MACRO_FIRST(first, ...) first
#define MACRO_SECOND(first, second, ...) second
#define MACRO_IS_PROBE(...) EXPAND_(MACRO_SECOND(__VA_ARGS__, 0))
#define MACRO_NOT(x) MACRO_IS_PROBE(MACRO_CONCAT(MACRO_NOT_, x))
#define MACRO_BOOL_(x) MACRO_NOT(MACRO_NOT(x))
#define MACRO_HAS_ARGS(...) EXPAND_(MACRO_BOOL_(MACRO_FIRST(MACRO_END_OF_ARGUMENTS_ __VA_ARGS__)()))
 
#if MACRO_HAS_ARGS() != 0
#error "MACRO_HAS_ARGS() != 0"
#endif
#if MACRO_HAS_ARGS(1) != 1
#error "MACRO_HAS_ARGS(1) != 1"
#endif
 
#define MACRO_IIF_1_ELSE(...)
#define MACRO_IIF_0_ELSE(...) __VA_ARGS__
#define MACRO_IIF_1(...) __VA_ARGS__ MACRO_IIF_1_ELSE
#define MACRO_IIF_0(...) MACRO_IIF_0_ELSE
#define MACRO_IIF_ELSE_(COND) MACRO_CONCAT(MACRO_IIF_, COND)
#define MACRO_IIF_ELSE(COND) MACRO_IIF_ELSE_(MACRO_BOOL_(COND))
#define ARG_COUNT(...) MACRO_IIF_ELSE_(MACRO_HAS_ARGS(__VA_ARGS__))(ARG_COUNT_TEST(__VA_ARGS__))(0)
#endif
 
#if ARG_COUNT() != 0
#error "ARG_COUNT() != 0"
#endif
#if ARG_COUNT(1,1) != 2
#error "ARG_COUNT(1,1) != 2"
#endif
#if ARG_COUNT(1) != 1
#error "ARG_COUNT(1) != 1"
#endif
 
#define ARG_OP(...) MACRO_CONCAT(ARG_OP_, ARG_COUNT(__VA_ARGS__))
 
#define MACRO_FOR_EACH(op, sep, ...) EXPAND_(ARG_OP(EXPAND_(__VA_ARGS__))(op, sep, ##__VA_ARGS__))
 
#define ARG_COMMA ,
#define FOR_EACH_COMMA(op, ...) EXPAND_(ARG_OP(EXPAND_(__VA_ARGS__))(op, ARG_COMMA, ##__VA_ARGS__))
// FOR_EACH_COMMA(MACRO, 1, 2, 3, 4)
// -->
// MICRO(1),MICRO(2),MICRO(3),MICRO(4)
 
#define WRAP_CALL(call, op, ...) call(EXPAND_(FOR_EACH_COMMA(op, ##__VA_ARGS__)))
// WRAP_CALL(call, MACRO, 1, 2, 3, 4)
// -->
// call(MACRO(1), MACRO(2), MACRO(3), MACRO(4))
 
// cannot use MACRO_FOR_EACH on gcc and clang?
#define WRAP_CALL1(call, op, ...) EXPAND_(ARG_OP(EXPAND_(__VA_ARGS__))(op, ARG_COMMA, ##__VA_ARGS__))
 
#define SEPERATOR_ARGS(sep, ...) EXPAND_(MACRO_FOR_EACH(EXPAND_, sep, ##__VA_ARGS__))
// std::cout <<  SEPERATOR_ARGS(<<, 1, 2, 3, std::endl)
//               -->
// std::cout <<  1 << 2 << 3 << std::endl
 
#define FOR_EACH_CALL(call, ...) EXPAND_(MACRO_FOR_EACH(call, ;, ##__VA_ARGS__))
// FOR_EACH_CALL(MACRO, 1, 2, 3, 4)
// -->
// MACRO(1); MACRO(2); MACRO(3); MACRO(4)
 
#define CHAIN_CALL(call,...) EXPAND_(MACRO_FOR_EACH(call, ., ##__VA_ARGS__))
// CHAIN_CALL(MACRO, 1, 2, 3, 4)
// -->
// MACRO(1).MACRO(2).MACRO(3).MACRO(4)
// clang-format on


//https://github.com/yabadabu/for_each
//----------------------------------------------------
#define EXPAND(x)     x
#define DEF_AUX_NARGS(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21,x22,x23,x24,x25,x26,x27,x28,x29,x30,x31,x32,x33,x34,x35,x36,x37,x38,x39,x40,x41,x42,x43,x44,x45,x46,x47,x48,x49,x50,x51,x52,x53,x54,x55,x56,x57,x58,x59,x60,x61,x62,x63,x64,VAL, ... )   VAL
#define NARGS(...)          EXPAND(DEF_AUX_NARGS(__VA_ARGS__,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))

// --------------------------------------------------
#define FE0_1(what,x)       EXPAND(what(x))
#define FE0_2(what,x, ...)  EXPAND(what(x)FE0_1(what, __VA_ARGS__))
#define FE0_3(what,x, ...)  EXPAND(what(x)FE0_2(what, __VA_ARGS__))
#define FE0_4(what,x, ...)  EXPAND(what(x)FE0_3(what, __VA_ARGS__))
#define FE0_5(what,x, ...)  EXPAND(what(x)FE0_4(what, __VA_ARGS__))
#define FE0_6(what,x, ...)  EXPAND(what(x)FE0_5(what, __VA_ARGS__))
#define FE0_7(what,x, ...)  EXPAND(what(x)FE0_6(what, __VA_ARGS__))
#define FE0_8(what,x, ...)  EXPAND(what(x)FE0_7(what, __VA_ARGS__))

#define REPEAT0_(...)       EXPAND(DEF_AUX_NARGS(__VA_ARGS__,FE0_8,FE0_7,FE0_6,FE0_5,FE0_4,FE0_3,FE0_2,FE0_1,0))
#define FOR_EACH(what,...)  EXPAND(REPEAT0_(__VA_ARGS__)(what,__VA_ARGS__))

// --------------------------------------------------
#define FE1_1(what,x, y)       EXPAND(what(x,y))
#define FE1_2(what,x, y, ...)  EXPAND(what(x,y)FE1_1(what,x, __VA_ARGS__))
#define FE1_3(what,x, y, ...)  EXPAND(what(x,y)FE1_2(what,x, __VA_ARGS__))
#define FE1_4(what,x, y, ...)  EXPAND(what(x,y)FE1_3(what,x, __VA_ARGS__))
#define FE1_5(what,x, y, ...)  EXPAND(what(x,y)FE1_4(what,x, __VA_ARGS__))
#define FE1_6(what,x, y, ...)  EXPAND(what(x,y)FE1_5(what,x, __VA_ARGS__))
#define FE1_7(what,x, y, ...)  EXPAND(what(x,y)FE1_6(what,x, __VA_ARGS__))
#define FE1_8(what,x, y, ...)  EXPAND(what(x,y)FE1_7(what,x, __VA_ARGS__))
#define FE1_9(what,x, y, ...)  EXPAND(what(x,y)FE1_8(what,x, __VA_ARGS__))
#define FE1_10(what,x, y, ...)  EXPAND(what(x,y)FE1_9(what,x, __VA_ARGS__))
#define FE1_11(what,x, y, ...)  EXPAND(what(x,y)FE1_10(what,x, __VA_ARGS__))
#define FE1_12(what,x, y, ...)  EXPAND(what(x,y)FE1_11(what,x, __VA_ARGS__))
#define FE1_13(what,x, y, ...)  EXPAND(what(x,y)FE1_12(what,x, __VA_ARGS__))
#define FE1_14(what,x, y, ...)  EXPAND(what(x,y)FE1_13(what,x, __VA_ARGS__))
#define FE1_15(what,x, y, ...)  EXPAND(what(x,y)FE1_14(what,x, __VA_ARGS__))
#define FE1_16(what,x, y, ...)  EXPAND(what(x,y)FE1_15(what,x, __VA_ARGS__))
#define FE1_17(what,x, y, ...)  EXPAND(what(x,y)FE1_16(what,x, __VA_ARGS__))
#define FE1_18(what,x, y, ...)  EXPAND(what(x,y)FE1_17(what,x, __VA_ARGS__))
#define FE1_19(what,x, y, ...)  EXPAND(what(x,y)FE1_18(what,x, __VA_ARGS__))
#define FE1_20(what,x, y, ...)  EXPAND(what(x,y)FE1_19(what,x, __VA_ARGS__))
#define FE1_21(what,x, y, ...)  EXPAND(what(x,y)FE1_20(what,x, __VA_ARGS__))
#define FE1_22(what,x, y, ...)  EXPAND(what(x,y)FE1_21(what,x, __VA_ARGS__))
#define FE1_23(what,x, y, ...)  EXPAND(what(x,y)FE1_22(what,x, __VA_ARGS__))
#define FE1_24(what,x, y, ...)  EXPAND(what(x,y)FE1_23(what,x, __VA_ARGS__))
#define FE1_25(what,x, y, ...)  EXPAND(what(x,y)FE1_24(what,x, __VA_ARGS__))
#define FE1_26(what,x, y, ...)  EXPAND(what(x,y)FE1_25(what,x, __VA_ARGS__))
#define FE1_27(what,x, y, ...)  EXPAND(what(x,y)FE1_26(what,x, __VA_ARGS__))
#define FE1_28(what,x, y, ...)  EXPAND(what(x,y)FE1_27(what,x, __VA_ARGS__))
#define FE1_29(what,x, y, ...)  EXPAND(what(x,y)FE1_28(what,x, __VA_ARGS__))
#define FE1_30(what,x, y, ...)  EXPAND(what(x,y)FE1_29(what,x, __VA_ARGS__))
#define FE1_31(what,x, y, ...)  EXPAND(what(x,y)FE1_30(what,x, __VA_ARGS__))
#define FE1_32(what,x, y, ...)  EXPAND(what(x,y)FE1_31(what,x, __VA_ARGS__))
#define FE1_33(what,x, y, ...)  EXPAND(what(x,y)FE1_32(what,x, __VA_ARGS__))
#define FE1_34(what,x, y, ...)  EXPAND(what(x,y)FE1_33(what,x, __VA_ARGS__))
#define FE1_35(what,x, y, ...)  EXPAND(what(x,y)FE1_34(what,x, __VA_ARGS__))
#define FE1_36(what,x, y, ...)  EXPAND(what(x,y)FE1_35(what,x, __VA_ARGS__))
#define FE1_37(what,x, y, ...)  EXPAND(what(x,y)FE1_36(what,x, __VA_ARGS__))
#define FE1_38(what,x, y, ...)  EXPAND(what(x,y)FE1_37(what,x, __VA_ARGS__))
#define FE1_39(what,x, y, ...)  EXPAND(what(x,y)FE1_38(what,x, __VA_ARGS__))
#define FE1_40(what,x, y, ...)  EXPAND(what(x,y)FE1_39(what,x, __VA_ARGS__))
#define FE1_41(what,x, y, ...)  EXPAND(what(x,y)FE1_40(what,x, __VA_ARGS__))
#define FE1_42(what,x, y, ...)  EXPAND(what(x,y)FE1_41(what,x, __VA_ARGS__))
#define FE1_43(what,x, y, ...)  EXPAND(what(x,y)FE1_42(what,x, __VA_ARGS__))
#define FE1_44(what,x, y, ...)  EXPAND(what(x,y)FE1_43(what,x, __VA_ARGS__))
#define FE1_45(what,x, y, ...)  EXPAND(what(x,y)FE1_44(what,x, __VA_ARGS__))
#define FE1_46(what,x, y, ...)  EXPAND(what(x,y)FE1_45(what,x, __VA_ARGS__))
#define FE1_47(what,x, y, ...)  EXPAND(what(x,y)FE1_46(what,x, __VA_ARGS__))
#define FE1_48(what,x, y, ...)  EXPAND(what(x,y)FE1_47(what,x, __VA_ARGS__))
#define FE1_49(what,x, y, ...)  EXPAND(what(x,y)FE1_48(what,x, __VA_ARGS__))
#define FE1_50(what,x, y, ...)  EXPAND(what(x,y)FE1_49(what,x, __VA_ARGS__))
#define FE1_51(what,x, y, ...)  EXPAND(what(x,y)FE1_50(what,x, __VA_ARGS__))
#define FE1_52(what,x, y, ...)  EXPAND(what(x,y)FE1_51(what,x, __VA_ARGS__))
#define FE1_53(what,x, y, ...)  EXPAND(what(x,y)FE1_52(what,x, __VA_ARGS__))
#define FE1_54(what,x, y, ...)  EXPAND(what(x,y)FE1_53(what,x, __VA_ARGS__))
#define FE1_55(what,x, y, ...)  EXPAND(what(x,y)FE1_54(what,x, __VA_ARGS__))
#define FE1_56(what,x, y, ...)  EXPAND(what(x,y)FE1_55(what,x, __VA_ARGS__))
#define FE1_57(what,x, y, ...)  EXPAND(what(x,y)FE1_56(what,x, __VA_ARGS__))
#define FE1_58(what,x, y, ...)  EXPAND(what(x,y)FE1_57(what,x, __VA_ARGS__))
#define FE1_59(what,x, y, ...)  EXPAND(what(x,y)FE1_58(what,x, __VA_ARGS__))
#define FE1_60(what,x, y, ...)  EXPAND(what(x,y)FE1_59(what,x, __VA_ARGS__))
#define FE1_61(what,x, y, ...)  EXPAND(what(x,y)FE1_60(what,x, __VA_ARGS__))
#define FE1_62(what,x, y, ...)  EXPAND(what(x,y)FE1_61(what,x, __VA_ARGS__))
#define FE1_63(what,x, y, ...)  EXPAND(what(x,y)FE1_62(what,x, __VA_ARGS__))
#define FE1_64(what,x, y, ...)  EXPAND(what(x,y)FE1_63(what,x, __VA_ARGS__))


#define REPEAT1_(...)  EXPAND(DEF_AUX_NARGS(__VA_ARGS__,FE1_64,FE1_63,FE1_62,FE1_61,FE1_60,FE1_59,FE1_58,FE1_57,FE1_56,FE1_55,FE1_54,FE1_53,FE1_52,FE1_51,FE1_50,FE1_49,FE1_48,FE1_47,FE1_46,FE1_45,FE1_44,FE1_43,FE1_42,FE1_41,FE1_40,FE1_39,FE1_38,FE1_37,FE1_36,FE1_35,FE1_34,FE1_33,FE1_32,FE1_31,FE1_30,FE1_29,FE1_28,FE1_27,FE1_26,FE1_25,FE1_24,FE1_23,FE1_22,FE1_21,FE1_20,FE1_19,FE1_18,FE1_17,FE1_16,FE1_15,FE1_14,FE1_13,FE1_12,FE1_11,FE1_10,FE1_9,FE1_8,FE1_7,FE1_6,FE1_5,FE1_4,FE1_3,FE1_2,FE1_1,0))
#define FOR_EACH_PIVOT_1ST_ARG(what,arg0,...)  EXPAND(REPEAT1_(__VA_ARGS__)(what,arg0,__VA_ARGS__))

// --------------------------------------------------
#define APPLY1(FN,x,y)       EXPAND(FN(x,y))
#define APPLY2(FN,x,y,...)   EXPAND(FN(x,y)APPLY1(FN,__VA_ARGS__))
#define APPLY3(FN,x,y,...)   EXPAND(FN(x,y)APPLY2(FN,__VA_ARGS__))
#define APPLY4(FN,x,y,...)   EXPAND(FN(x,y)APPLY3(FN,__VA_ARGS__))
#define APPLY5(FN,x,y,...)   EXPAND(FN(x,y)APPLY4(FN,__VA_ARGS__))
#define APPLY6(FN,x,y,...)   EXPAND(FN(x,y)APPLY5(FN,__VA_ARGS__))
#define APPLY7(FN,x,y,...)   EXPAND(FN(x,y)APPLY6(FN,__VA_ARGS__))
#define APPLY8(FN,x,y,...)   EXPAND(FN(x,y)APPLY7(FN,__VA_ARGS__))
#define APPLY9(FN,x,y,...)   EXPAND(FN(x,y)APPLY8(FN,__VA_ARGS__))
#define APPLY10(FN,x,y,...)  EXPAND(FN(x,y)APPLY9(FN,__VA_ARGS__))
#define APPLY11(FN,x,y,...)  EXPAND(FN(x,y)APPLY10(FN,__VA_ARGS__))
#define APPLY12(FN,x,y,...)  EXPAND(FN(x,y)APPLY11(FN,__VA_ARGS__))
#define APPLY13(FN,x,y,...)  EXPAND(FN(x,y)APPLY12(FN,__VA_ARGS__))
#define APPLY14(FN,x,y,...)  EXPAND(FN(x,y)APPLY13(FN,__VA_ARGS__))
#define APPLY15(FN,x,y,...)  EXPAND(FN(x,y)APPLY14(FN,__VA_ARGS__))
#define APPLY16(FN,x,y,...)  EXPAND(FN(x,y)APPLY15(FN,__VA_ARGS__))
#define APPLY17(FN,x,y,...)  EXPAND(FN(x,y)APPLY16(FN,__VA_ARGS__))
#define APPLY18(FN,x,y,...)  EXPAND(FN(x,y)APPLY17(FN,__VA_ARGS__))
#define APPLY19(FN,x,y,...)  EXPAND(FN(x,y)APPLY18(FN,__VA_ARGS__))
#define APPLY20(FN,x,y,...)  EXPAND(FN(x,y)APPLY19(FN,__VA_ARGS__))
#define APPLY21(FN,x,y,...)  EXPAND(FN(x,y)APPLY20(FN,__VA_ARGS__))
#define APPLY22(FN,x,y,...)  EXPAND(FN(x,y)APPLY21(FN,__VA_ARGS__))
#define APPLY23(FN,x,y,...)  EXPAND(FN(x,y)APPLY22(FN,__VA_ARGS__))
#define APPLY24(FN,x,y,...)  EXPAND(FN(x,y)APPLY23(FN,__VA_ARGS__))
#define APPLY25(FN,x,y,...)  EXPAND(FN(x,y)APPLY24(FN,__VA_ARGS__))
#define APPLY26(FN,x,y,...)  EXPAND(FN(x,y)APPLY25(FN,__VA_ARGS__))
#define APPLY27(FN,x,y,...)  EXPAND(FN(x,y)APPLY26(FN,__VA_ARGS__))
#define APPLY28(FN,x,y,...)  EXPAND(FN(x,y)APPLY27(FN,__VA_ARGS__))
#define APPLY29(FN,x,y,...)  EXPAND(FN(x,y)APPLY28(FN,__VA_ARGS__))
#define APPLY30(FN,x,y,...)  EXPAND(FN(x,y)APPLY29(FN,__VA_ARGS__))
#define APPLY31(FN,x,y,...)  EXPAND(FN(x,y)APPLY30(FN,__VA_ARGS__))
#define APPLY32(FN,x,y,...)  EXPAND(FN(x,y)APPLY31(FN,__VA_ARGS__))

#define NPAIRARGS(...)        EXPAND(DEF_AUX_NARGS(__VA_ARGS__,32,32,31,31,30,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1))
#define APPLYNPAIRARGS(...)   EXPAND(DEF_AUX_NARGS(__VA_ARGS__,APPLY32,APPLY32,APPLY31,APPLY31,APPLY30,APPLY30,APPLY29,APPLY29,APPLY28,APPLY28,APPLY27,APPLY27,APPLY26,APPLY26,APPLY25,APPLY25,APPLY24,APPLY24,APPLY23,APPLY23,APPLY22,APPLY22,APPLY21,APPLY21,APPLY20,APPLY20,APPLY19,APPLY19,APPLY18,APPLY18,APPLY17,APPLY17,APPLY16,APPLY16,APPLY15,APPLY15,APPLY14,APPLY14,APPLY13,APPLY13,APPLY12,APPLY12,APPLY11,APPLY11,APPLY10,APPLY10,APPLY9,APPLY9,APPLY8,APPLY8,APPLY7,APPLY7,APPLY6,APPLY6,APPLY5,APPLY5,APPLY4,APPLY4,APPLY3,APPLY3,APPLY2,APPLY2,APPLY1,APPLY1))
#define FOR_EACH_PAIR(FN,...) EXPAND(APPLYNPAIRARGS(__VA_ARGS__)(FN,__VA_ARGS__))
// --------------------------------------------------
#endif
