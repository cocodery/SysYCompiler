
// Generated from SysY.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  SysYLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, Int = 4, Void = 5, Const = 6, Return = 7, 
    If = 8, Else = 9, While = 10, Break = 11, Continue = 12, Lparen = 13, 
    Rparen = 14, Lbrkt = 15, Rbrkt = 16, Lbrace = 17, Rbrace = 18, Comma = 19, 
    Semicolon = 20, Minus = 21, Addition = 22, Exclamation = 23, Multiplication = 24, 
    Division = 25, Modulo = 26, LAND = 27, LOR = 28, EQ = 29, NEQ = 30, 
    LT = 31, LE = 32, GT = 33, GE = 34, IntLiteral = 35, Identifier = 36, 
    WS = 37, LINE_COMMENT = 38, COMMENT = 39
  };

  SysYLexer(antlr4::CharStream *input);
  ~SysYLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

