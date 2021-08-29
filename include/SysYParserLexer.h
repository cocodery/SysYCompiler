
// Generated from SysYParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  SysYParserLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, Int = 2, Void = 3, Const = 4, Return = 5, If = 6, Else = 7, 
    While = 8, Break = 9, Continue = 10, Lparen = 11, Rparen = 12, Lbrkt = 13, 
    Rbrkt = 14, Lbrace = 15, Rbrace = 16, Comma = 17, Semicolon = 18, Minus = 19, 
    Addition = 20, Exclamation = 21, Multiplication = 22, Division = 23, 
    Modulo = 24, LAND = 25, LOR = 26, EQ = 27, NEQ = 28, LT = 29, LE = 30, 
    GT = 31, GE = 32, IntLiteral = 33, Identifier = 34, WS = 35, LINE_COMMENT = 36, 
    COMMENT = 37
  };

  explicit SysYParserLexer(antlr4::CharStream *input);
  ~SysYParserLexer();

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

