
// Generated from SysYParser.g4 by ANTLR 4.9.2


#include "SysYParserVisitor.h"

#include "SysYParserParser.h"


using namespace antlrcpp;
using namespace antlr4;

SysYParserParser::SysYParserParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SysYParserParser::~SysYParserParser() {
  delete _interpreter;
}

std::string SysYParserParser::getGrammarFileName() const {
  return "SysYParser.g4";
}

const std::vector<std::string>& SysYParserParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& SysYParserParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- CompUnitContext ------------------------------------------------------------------

SysYParserParser::CompUnitContext::CompUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::CompUnitContext::EOF() {
  return getToken(SysYParserParser::EOF, 0);
}

std::vector<SysYParserParser::DeclContext *> SysYParserParser::CompUnitContext::decl() {
  return getRuleContexts<SysYParserParser::DeclContext>();
}

SysYParserParser::DeclContext* SysYParserParser::CompUnitContext::decl(size_t i) {
  return getRuleContext<SysYParserParser::DeclContext>(i);
}

std::vector<SysYParserParser::FuncDefContext *> SysYParserParser::CompUnitContext::funcDef() {
  return getRuleContexts<SysYParserParser::FuncDefContext>();
}

SysYParserParser::FuncDefContext* SysYParserParser::CompUnitContext::funcDef(size_t i) {
  return getRuleContext<SysYParserParser::FuncDefContext>(i);
}


size_t SysYParserParser::CompUnitContext::getRuleIndex() const {
  return SysYParserParser::RuleCompUnit;
}


antlrcpp::Any SysYParserParser::CompUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitCompUnit(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::CompUnitContext* SysYParserParser::compUnit() {
  CompUnitContext *_localctx = _tracker.createInstance<CompUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, SysYParserParser::RuleCompUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SysYParserParser::Int)
      | (1ULL << SysYParserParser::Void)
      | (1ULL << SysYParserParser::Const))) != 0)) {
      setState(66);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(64);
        decl();
        break;
      }

      case 2: {
        setState(65);
        funcDef();
        break;
      }

      default:
        break;
      }
      setState(70);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(71);
    match(SysYParserParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

SysYParserParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::ConstDeclContext* SysYParserParser::DeclContext::constDecl() {
  return getRuleContext<SysYParserParser::ConstDeclContext>(0);
}

SysYParserParser::VarDeclContext* SysYParserParser::DeclContext::varDecl() {
  return getRuleContext<SysYParserParser::VarDeclContext>(0);
}


size_t SysYParserParser::DeclContext::getRuleIndex() const {
  return SysYParserParser::RuleDecl;
}


antlrcpp::Any SysYParserParser::DeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitDecl(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::DeclContext* SysYParserParser::decl() {
  DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
  enterRule(_localctx, 2, SysYParserParser::RuleDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(75);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParserParser::Const: {
        enterOuterAlt(_localctx, 1);
        setState(73);
        constDecl();
        break;
      }

      case SysYParserParser::Int: {
        enterOuterAlt(_localctx, 2);
        setState(74);
        varDecl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDeclContext ------------------------------------------------------------------

SysYParserParser::ConstDeclContext::ConstDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::ConstDeclContext::Const() {
  return getToken(SysYParserParser::Const, 0);
}

SysYParserParser::BTypeContext* SysYParserParser::ConstDeclContext::bType() {
  return getRuleContext<SysYParserParser::BTypeContext>(0);
}

std::vector<SysYParserParser::ConstDefContext *> SysYParserParser::ConstDeclContext::constDef() {
  return getRuleContexts<SysYParserParser::ConstDefContext>();
}

SysYParserParser::ConstDefContext* SysYParserParser::ConstDeclContext::constDef(size_t i) {
  return getRuleContext<SysYParserParser::ConstDefContext>(i);
}

tree::TerminalNode* SysYParserParser::ConstDeclContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

std::vector<tree::TerminalNode *> SysYParserParser::ConstDeclContext::Comma() {
  return getTokens(SysYParserParser::Comma);
}

tree::TerminalNode* SysYParserParser::ConstDeclContext::Comma(size_t i) {
  return getToken(SysYParserParser::Comma, i);
}


size_t SysYParserParser::ConstDeclContext::getRuleIndex() const {
  return SysYParserParser::RuleConstDecl;
}


antlrcpp::Any SysYParserParser::ConstDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitConstDecl(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::ConstDeclContext* SysYParserParser::constDecl() {
  ConstDeclContext *_localctx = _tracker.createInstance<ConstDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, SysYParserParser::RuleConstDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(77);
    match(SysYParserParser::Const);
    setState(78);
    bType();
    setState(79);
    constDef();
    setState(84);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParserParser::Comma) {
      setState(80);
      match(SysYParserParser::Comma);
      setState(81);
      constDef();
      setState(86);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(87);
    match(SysYParserParser::Semicolon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BTypeContext ------------------------------------------------------------------

SysYParserParser::BTypeContext::BTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::BTypeContext::Int() {
  return getToken(SysYParserParser::Int, 0);
}


size_t SysYParserParser::BTypeContext::getRuleIndex() const {
  return SysYParserParser::RuleBType;
}


antlrcpp::Any SysYParserParser::BTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitBType(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::BTypeContext* SysYParserParser::bType() {
  BTypeContext *_localctx = _tracker.createInstance<BTypeContext>(_ctx, getState());
  enterRule(_localctx, 6, SysYParserParser::RuleBType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(89);
    match(SysYParserParser::Int);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDefContext ------------------------------------------------------------------

SysYParserParser::ConstDefContext::ConstDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::ConstDefContext::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

SysYParserParser::ConstInitValContext* SysYParserParser::ConstDefContext::constInitVal() {
  return getRuleContext<SysYParserParser::ConstInitValContext>(0);
}

std::vector<tree::TerminalNode *> SysYParserParser::ConstDefContext::Lbrkt() {
  return getTokens(SysYParserParser::Lbrkt);
}

tree::TerminalNode* SysYParserParser::ConstDefContext::Lbrkt(size_t i) {
  return getToken(SysYParserParser::Lbrkt, i);
}

std::vector<SysYParserParser::ConstExpContext *> SysYParserParser::ConstDefContext::constExp() {
  return getRuleContexts<SysYParserParser::ConstExpContext>();
}

SysYParserParser::ConstExpContext* SysYParserParser::ConstDefContext::constExp(size_t i) {
  return getRuleContext<SysYParserParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::ConstDefContext::Rbrkt() {
  return getTokens(SysYParserParser::Rbrkt);
}

tree::TerminalNode* SysYParserParser::ConstDefContext::Rbrkt(size_t i) {
  return getToken(SysYParserParser::Rbrkt, i);
}


size_t SysYParserParser::ConstDefContext::getRuleIndex() const {
  return SysYParserParser::RuleConstDef;
}


antlrcpp::Any SysYParserParser::ConstDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitConstDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::ConstDefContext* SysYParserParser::constDef() {
  ConstDefContext *_localctx = _tracker.createInstance<ConstDefContext>(_ctx, getState());
  enterRule(_localctx, 8, SysYParserParser::RuleConstDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(91);
    match(SysYParserParser::Identifier);
    setState(98);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParserParser::Lbrkt) {
      setState(92);
      match(SysYParserParser::Lbrkt);
      setState(93);
      constExp();
      setState(94);
      match(SysYParserParser::Rbrkt);
      setState(100);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(101);
    match(SysYParserParser::T__0);
    setState(102);
    constInitVal();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstInitValContext ------------------------------------------------------------------

SysYParserParser::ConstInitValContext::ConstInitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::ConstInitValContext::getRuleIndex() const {
  return SysYParserParser::RuleConstInitVal;
}

void SysYParserParser::ConstInitValContext::copyFrom(ConstInitValContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ListConstInitValContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::ListConstInitValContext::Lbrace() {
  return getToken(SysYParserParser::Lbrace, 0);
}

tree::TerminalNode* SysYParserParser::ListConstInitValContext::Rbrace() {
  return getToken(SysYParserParser::Rbrace, 0);
}

std::vector<SysYParserParser::ConstInitValContext *> SysYParserParser::ListConstInitValContext::constInitVal() {
  return getRuleContexts<SysYParserParser::ConstInitValContext>();
}

SysYParserParser::ConstInitValContext* SysYParserParser::ListConstInitValContext::constInitVal(size_t i) {
  return getRuleContext<SysYParserParser::ConstInitValContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::ListConstInitValContext::Comma() {
  return getTokens(SysYParserParser::Comma);
}

tree::TerminalNode* SysYParserParser::ListConstInitValContext::Comma(size_t i) {
  return getToken(SysYParserParser::Comma, i);
}

SysYParserParser::ListConstInitValContext::ListConstInitValContext(ConstInitValContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ListConstInitValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitListConstInitVal(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ScalarConstInitValContext ------------------------------------------------------------------

SysYParserParser::ConstExpContext* SysYParserParser::ScalarConstInitValContext::constExp() {
  return getRuleContext<SysYParserParser::ConstExpContext>(0);
}

SysYParserParser::ScalarConstInitValContext::ScalarConstInitValContext(ConstInitValContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ScalarConstInitValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitScalarConstInitVal(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::ConstInitValContext* SysYParserParser::constInitVal() {
  ConstInitValContext *_localctx = _tracker.createInstance<ConstInitValContext>(_ctx, getState());
  enterRule(_localctx, 10, SysYParserParser::RuleConstInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(117);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParserParser::Lparen:
      case SysYParserParser::Minus:
      case SysYParserParser::Addition:
      case SysYParserParser::Exclamation:
      case SysYParserParser::IntLiteral:
      case SysYParserParser::Identifier: {
        _localctx = dynamic_cast<ConstInitValContext *>(_tracker.createInstance<SysYParserParser::ScalarConstInitValContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(104);
        constExp();
        break;
      }

      case SysYParserParser::Lbrace: {
        _localctx = dynamic_cast<ConstInitValContext *>(_tracker.createInstance<SysYParserParser::ListConstInitValContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(105);
        match(SysYParserParser::Lbrace);
        setState(114);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SysYParserParser::Lparen)
          | (1ULL << SysYParserParser::Lbrace)
          | (1ULL << SysYParserParser::Minus)
          | (1ULL << SysYParserParser::Addition)
          | (1ULL << SysYParserParser::Exclamation)
          | (1ULL << SysYParserParser::IntLiteral)
          | (1ULL << SysYParserParser::Identifier))) != 0)) {
          setState(106);
          constInitVal();
          setState(111);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SysYParserParser::Comma) {
            setState(107);
            match(SysYParserParser::Comma);
            setState(108);
            constInitVal();
            setState(113);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(116);
        match(SysYParserParser::Rbrace);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

SysYParserParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::BTypeContext* SysYParserParser::VarDeclContext::bType() {
  return getRuleContext<SysYParserParser::BTypeContext>(0);
}

std::vector<SysYParserParser::VarDefContext *> SysYParserParser::VarDeclContext::varDef() {
  return getRuleContexts<SysYParserParser::VarDefContext>();
}

SysYParserParser::VarDefContext* SysYParserParser::VarDeclContext::varDef(size_t i) {
  return getRuleContext<SysYParserParser::VarDefContext>(i);
}

tree::TerminalNode* SysYParserParser::VarDeclContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

std::vector<tree::TerminalNode *> SysYParserParser::VarDeclContext::Comma() {
  return getTokens(SysYParserParser::Comma);
}

tree::TerminalNode* SysYParserParser::VarDeclContext::Comma(size_t i) {
  return getToken(SysYParserParser::Comma, i);
}


size_t SysYParserParser::VarDeclContext::getRuleIndex() const {
  return SysYParserParser::RuleVarDecl;
}


antlrcpp::Any SysYParserParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::VarDeclContext* SysYParserParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 12, SysYParserParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(119);
    bType();
    setState(120);
    varDef();
    setState(125);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParserParser::Comma) {
      setState(121);
      match(SysYParserParser::Comma);
      setState(122);
      varDef();
      setState(127);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(128);
    match(SysYParserParser::Semicolon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

SysYParserParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::VarDefContext::getRuleIndex() const {
  return SysYParserParser::RuleVarDef;
}

void SysYParserParser::VarDefContext::copyFrom(VarDefContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- UninitVarDefContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::UninitVarDefContext::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

std::vector<tree::TerminalNode *> SysYParserParser::UninitVarDefContext::Lbrkt() {
  return getTokens(SysYParserParser::Lbrkt);
}

tree::TerminalNode* SysYParserParser::UninitVarDefContext::Lbrkt(size_t i) {
  return getToken(SysYParserParser::Lbrkt, i);
}

std::vector<SysYParserParser::ConstExpContext *> SysYParserParser::UninitVarDefContext::constExp() {
  return getRuleContexts<SysYParserParser::ConstExpContext>();
}

SysYParserParser::ConstExpContext* SysYParserParser::UninitVarDefContext::constExp(size_t i) {
  return getRuleContext<SysYParserParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::UninitVarDefContext::Rbrkt() {
  return getTokens(SysYParserParser::Rbrkt);
}

tree::TerminalNode* SysYParserParser::UninitVarDefContext::Rbrkt(size_t i) {
  return getToken(SysYParserParser::Rbrkt, i);
}

SysYParserParser::UninitVarDefContext::UninitVarDefContext(VarDefContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::UninitVarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitUninitVarDef(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InitVarDefContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::InitVarDefContext::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

SysYParserParser::InitValContext* SysYParserParser::InitVarDefContext::initVal() {
  return getRuleContext<SysYParserParser::InitValContext>(0);
}

std::vector<tree::TerminalNode *> SysYParserParser::InitVarDefContext::Lbrkt() {
  return getTokens(SysYParserParser::Lbrkt);
}

tree::TerminalNode* SysYParserParser::InitVarDefContext::Lbrkt(size_t i) {
  return getToken(SysYParserParser::Lbrkt, i);
}

std::vector<SysYParserParser::ConstExpContext *> SysYParserParser::InitVarDefContext::constExp() {
  return getRuleContexts<SysYParserParser::ConstExpContext>();
}

SysYParserParser::ConstExpContext* SysYParserParser::InitVarDefContext::constExp(size_t i) {
  return getRuleContext<SysYParserParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::InitVarDefContext::Rbrkt() {
  return getTokens(SysYParserParser::Rbrkt);
}

tree::TerminalNode* SysYParserParser::InitVarDefContext::Rbrkt(size_t i) {
  return getToken(SysYParserParser::Rbrkt, i);
}

SysYParserParser::InitVarDefContext::InitVarDefContext(VarDefContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::InitVarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitInitVarDef(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::VarDefContext* SysYParserParser::varDef() {
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 14, SysYParserParser::RuleVarDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(152);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<VarDefContext *>(_tracker.createInstance<SysYParserParser::UninitVarDefContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(130);
      match(SysYParserParser::Identifier);
      setState(137);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParserParser::Lbrkt) {
        setState(131);
        match(SysYParserParser::Lbrkt);
        setState(132);
        constExp();
        setState(133);
        match(SysYParserParser::Rbrkt);
        setState(139);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = dynamic_cast<VarDefContext *>(_tracker.createInstance<SysYParserParser::InitVarDefContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(140);
      match(SysYParserParser::Identifier);
      setState(147);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParserParser::Lbrkt) {
        setState(141);
        match(SysYParserParser::Lbrkt);
        setState(142);
        constExp();
        setState(143);
        match(SysYParserParser::Rbrkt);
        setState(149);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(150);
      match(SysYParserParser::T__0);
      setState(151);
      initVal();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitValContext ------------------------------------------------------------------

SysYParserParser::InitValContext::InitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::InitValContext::getRuleIndex() const {
  return SysYParserParser::RuleInitVal;
}

void SysYParserParser::InitValContext::copyFrom(InitValContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ScalarInitValContext ------------------------------------------------------------------

SysYParserParser::ExpContext* SysYParserParser::ScalarInitValContext::exp() {
  return getRuleContext<SysYParserParser::ExpContext>(0);
}

SysYParserParser::ScalarInitValContext::ScalarInitValContext(InitValContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ScalarInitValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitScalarInitVal(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ListInitvalContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::ListInitvalContext::Lbrace() {
  return getToken(SysYParserParser::Lbrace, 0);
}

tree::TerminalNode* SysYParserParser::ListInitvalContext::Rbrace() {
  return getToken(SysYParserParser::Rbrace, 0);
}

std::vector<SysYParserParser::InitValContext *> SysYParserParser::ListInitvalContext::initVal() {
  return getRuleContexts<SysYParserParser::InitValContext>();
}

SysYParserParser::InitValContext* SysYParserParser::ListInitvalContext::initVal(size_t i) {
  return getRuleContext<SysYParserParser::InitValContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::ListInitvalContext::Comma() {
  return getTokens(SysYParserParser::Comma);
}

tree::TerminalNode* SysYParserParser::ListInitvalContext::Comma(size_t i) {
  return getToken(SysYParserParser::Comma, i);
}

SysYParserParser::ListInitvalContext::ListInitvalContext(InitValContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ListInitvalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitListInitval(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::InitValContext* SysYParserParser::initVal() {
  InitValContext *_localctx = _tracker.createInstance<InitValContext>(_ctx, getState());
  enterRule(_localctx, 16, SysYParserParser::RuleInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(167);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParserParser::Lparen:
      case SysYParserParser::Minus:
      case SysYParserParser::Addition:
      case SysYParserParser::Exclamation:
      case SysYParserParser::IntLiteral:
      case SysYParserParser::Identifier: {
        _localctx = dynamic_cast<InitValContext *>(_tracker.createInstance<SysYParserParser::ScalarInitValContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(154);
        exp();
        break;
      }

      case SysYParserParser::Lbrace: {
        _localctx = dynamic_cast<InitValContext *>(_tracker.createInstance<SysYParserParser::ListInitvalContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(155);
        match(SysYParserParser::Lbrace);
        setState(164);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SysYParserParser::Lparen)
          | (1ULL << SysYParserParser::Lbrace)
          | (1ULL << SysYParserParser::Minus)
          | (1ULL << SysYParserParser::Addition)
          | (1ULL << SysYParserParser::Exclamation)
          | (1ULL << SysYParserParser::IntLiteral)
          | (1ULL << SysYParserParser::Identifier))) != 0)) {
          setState(156);
          initVal();
          setState(161);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SysYParserParser::Comma) {
            setState(157);
            match(SysYParserParser::Comma);
            setState(158);
            initVal();
            setState(163);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(166);
        match(SysYParserParser::Rbrace);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

SysYParserParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::FuncTypeContext* SysYParserParser::FuncDefContext::funcType() {
  return getRuleContext<SysYParserParser::FuncTypeContext>(0);
}

tree::TerminalNode* SysYParserParser::FuncDefContext::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

tree::TerminalNode* SysYParserParser::FuncDefContext::Lparen() {
  return getToken(SysYParserParser::Lparen, 0);
}

tree::TerminalNode* SysYParserParser::FuncDefContext::Rparen() {
  return getToken(SysYParserParser::Rparen, 0);
}

SysYParserParser::BlockContext* SysYParserParser::FuncDefContext::block() {
  return getRuleContext<SysYParserParser::BlockContext>(0);
}

SysYParserParser::FuncFParamsContext* SysYParserParser::FuncDefContext::funcFParams() {
  return getRuleContext<SysYParserParser::FuncFParamsContext>(0);
}


size_t SysYParserParser::FuncDefContext::getRuleIndex() const {
  return SysYParserParser::RuleFuncDef;
}


antlrcpp::Any SysYParserParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::FuncDefContext* SysYParserParser::funcDef() {
  FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
  enterRule(_localctx, 18, SysYParserParser::RuleFuncDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(169);
    funcType();
    setState(170);
    match(SysYParserParser::Identifier);
    setState(171);
    match(SysYParserParser::Lparen);
    setState(173);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SysYParserParser::Int) {
      setState(172);
      funcFParams();
    }
    setState(175);
    match(SysYParserParser::Rparen);
    setState(176);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncTypeContext ------------------------------------------------------------------

SysYParserParser::FuncTypeContext::FuncTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::FuncTypeContext::Void() {
  return getToken(SysYParserParser::Void, 0);
}

tree::TerminalNode* SysYParserParser::FuncTypeContext::Int() {
  return getToken(SysYParserParser::Int, 0);
}


size_t SysYParserParser::FuncTypeContext::getRuleIndex() const {
  return SysYParserParser::RuleFuncType;
}


antlrcpp::Any SysYParserParser::FuncTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitFuncType(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::FuncTypeContext* SysYParserParser::funcType() {
  FuncTypeContext *_localctx = _tracker.createInstance<FuncTypeContext>(_ctx, getState());
  enterRule(_localctx, 20, SysYParserParser::RuleFuncType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(178);
    _la = _input->LA(1);
    if (!(_la == SysYParserParser::Int

    || _la == SysYParserParser::Void)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamsContext ------------------------------------------------------------------

SysYParserParser::FuncFParamsContext::FuncFParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParserParser::FuncFParamContext *> SysYParserParser::FuncFParamsContext::funcFParam() {
  return getRuleContexts<SysYParserParser::FuncFParamContext>();
}

SysYParserParser::FuncFParamContext* SysYParserParser::FuncFParamsContext::funcFParam(size_t i) {
  return getRuleContext<SysYParserParser::FuncFParamContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::FuncFParamsContext::Comma() {
  return getTokens(SysYParserParser::Comma);
}

tree::TerminalNode* SysYParserParser::FuncFParamsContext::Comma(size_t i) {
  return getToken(SysYParserParser::Comma, i);
}


size_t SysYParserParser::FuncFParamsContext::getRuleIndex() const {
  return SysYParserParser::RuleFuncFParams;
}


antlrcpp::Any SysYParserParser::FuncFParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitFuncFParams(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::FuncFParamsContext* SysYParserParser::funcFParams() {
  FuncFParamsContext *_localctx = _tracker.createInstance<FuncFParamsContext>(_ctx, getState());
  enterRule(_localctx, 22, SysYParserParser::RuleFuncFParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(180);
    funcFParam();
    setState(185);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParserParser::Comma) {
      setState(181);
      match(SysYParserParser::Comma);
      setState(182);
      funcFParam();
      setState(187);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamContext ------------------------------------------------------------------

SysYParserParser::FuncFParamContext::FuncFParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::BTypeContext* SysYParserParser::FuncFParamContext::bType() {
  return getRuleContext<SysYParserParser::BTypeContext>(0);
}

tree::TerminalNode* SysYParserParser::FuncFParamContext::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

std::vector<tree::TerminalNode *> SysYParserParser::FuncFParamContext::Lbrkt() {
  return getTokens(SysYParserParser::Lbrkt);
}

tree::TerminalNode* SysYParserParser::FuncFParamContext::Lbrkt(size_t i) {
  return getToken(SysYParserParser::Lbrkt, i);
}

std::vector<tree::TerminalNode *> SysYParserParser::FuncFParamContext::Rbrkt() {
  return getTokens(SysYParserParser::Rbrkt);
}

tree::TerminalNode* SysYParserParser::FuncFParamContext::Rbrkt(size_t i) {
  return getToken(SysYParserParser::Rbrkt, i);
}

std::vector<SysYParserParser::ConstExpContext *> SysYParserParser::FuncFParamContext::constExp() {
  return getRuleContexts<SysYParserParser::ConstExpContext>();
}

SysYParserParser::ConstExpContext* SysYParserParser::FuncFParamContext::constExp(size_t i) {
  return getRuleContext<SysYParserParser::ConstExpContext>(i);
}


size_t SysYParserParser::FuncFParamContext::getRuleIndex() const {
  return SysYParserParser::RuleFuncFParam;
}


antlrcpp::Any SysYParserParser::FuncFParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitFuncFParam(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::FuncFParamContext* SysYParserParser::funcFParam() {
  FuncFParamContext *_localctx = _tracker.createInstance<FuncFParamContext>(_ctx, getState());
  enterRule(_localctx, 24, SysYParserParser::RuleFuncFParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    bType();
    setState(189);
    match(SysYParserParser::Identifier);
    setState(201);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SysYParserParser::Lbrkt) {
      setState(190);
      match(SysYParserParser::Lbrkt);
      setState(191);
      match(SysYParserParser::Rbrkt);
      setState(198);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParserParser::Lbrkt) {
        setState(192);
        match(SysYParserParser::Lbrkt);
        setState(193);
        constExp();
        setState(194);
        match(SysYParserParser::Rbrkt);
        setState(200);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

SysYParserParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::BlockContext::Lbrace() {
  return getToken(SysYParserParser::Lbrace, 0);
}

tree::TerminalNode* SysYParserParser::BlockContext::Rbrace() {
  return getToken(SysYParserParser::Rbrace, 0);
}

std::vector<SysYParserParser::BlockItemContext *> SysYParserParser::BlockContext::blockItem() {
  return getRuleContexts<SysYParserParser::BlockItemContext>();
}

SysYParserParser::BlockItemContext* SysYParserParser::BlockContext::blockItem(size_t i) {
  return getRuleContext<SysYParserParser::BlockItemContext>(i);
}


size_t SysYParserParser::BlockContext::getRuleIndex() const {
  return SysYParserParser::RuleBlock;
}


antlrcpp::Any SysYParserParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::BlockContext* SysYParserParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 26, SysYParserParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(203);
    match(SysYParserParser::Lbrace);
    setState(207);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SysYParserParser::Int)
      | (1ULL << SysYParserParser::Const)
      | (1ULL << SysYParserParser::Return)
      | (1ULL << SysYParserParser::If)
      | (1ULL << SysYParserParser::While)
      | (1ULL << SysYParserParser::Break)
      | (1ULL << SysYParserParser::Continue)
      | (1ULL << SysYParserParser::Lparen)
      | (1ULL << SysYParserParser::Lbrace)
      | (1ULL << SysYParserParser::Semicolon)
      | (1ULL << SysYParserParser::Minus)
      | (1ULL << SysYParserParser::Addition)
      | (1ULL << SysYParserParser::Exclamation)
      | (1ULL << SysYParserParser::IntLiteral)
      | (1ULL << SysYParserParser::Identifier))) != 0)) {
      setState(204);
      blockItem();
      setState(209);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(210);
    match(SysYParserParser::Rbrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

SysYParserParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::DeclContext* SysYParserParser::BlockItemContext::decl() {
  return getRuleContext<SysYParserParser::DeclContext>(0);
}

SysYParserParser::StmtContext* SysYParserParser::BlockItemContext::stmt() {
  return getRuleContext<SysYParserParser::StmtContext>(0);
}


size_t SysYParserParser::BlockItemContext::getRuleIndex() const {
  return SysYParserParser::RuleBlockItem;
}


antlrcpp::Any SysYParserParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::BlockItemContext* SysYParserParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 28, SysYParserParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(214);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParserParser::Int:
      case SysYParserParser::Const: {
        enterOuterAlt(_localctx, 1);
        setState(212);
        decl();
        break;
      }

      case SysYParserParser::Return:
      case SysYParserParser::If:
      case SysYParserParser::While:
      case SysYParserParser::Break:
      case SysYParserParser::Continue:
      case SysYParserParser::Lparen:
      case SysYParserParser::Lbrace:
      case SysYParserParser::Semicolon:
      case SysYParserParser::Minus:
      case SysYParserParser::Addition:
      case SysYParserParser::Exclamation:
      case SysYParserParser::IntLiteral:
      case SysYParserParser::Identifier: {
        enterOuterAlt(_localctx, 2);
        setState(213);
        stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

SysYParserParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::StmtContext::getRuleIndex() const {
  return SysYParserParser::RuleStmt;
}

void SysYParserParser::StmtContext::copyFrom(StmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- WhileStmtContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::WhileStmtContext::While() {
  return getToken(SysYParserParser::While, 0);
}

tree::TerminalNode* SysYParserParser::WhileStmtContext::Lparen() {
  return getToken(SysYParserParser::Lparen, 0);
}

SysYParserParser::CondContext* SysYParserParser::WhileStmtContext::cond() {
  return getRuleContext<SysYParserParser::CondContext>(0);
}

tree::TerminalNode* SysYParserParser::WhileStmtContext::Rparen() {
  return getToken(SysYParserParser::Rparen, 0);
}

SysYParserParser::StmtContext* SysYParserParser::WhileStmtContext::stmt() {
  return getRuleContext<SysYParserParser::StmtContext>(0);
}

SysYParserParser::WhileStmtContext::WhileStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::WhileStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitWhileStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BlockStmtContext ------------------------------------------------------------------

SysYParserParser::BlockContext* SysYParserParser::BlockStmtContext::block() {
  return getRuleContext<SysYParserParser::BlockContext>(0);
}

SysYParserParser::BlockStmtContext::BlockStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::BlockStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitBlockStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignmentContext ------------------------------------------------------------------

SysYParserParser::LValContext* SysYParserParser::AssignmentContext::lVal() {
  return getRuleContext<SysYParserParser::LValContext>(0);
}

SysYParserParser::ExpContext* SysYParserParser::AssignmentContext::exp() {
  return getRuleContext<SysYParserParser::ExpContext>(0);
}

tree::TerminalNode* SysYParserParser::AssignmentContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

SysYParserParser::AssignmentContext::AssignmentContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfStmt1Context ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::IfStmt1Context::If() {
  return getToken(SysYParserParser::If, 0);
}

tree::TerminalNode* SysYParserParser::IfStmt1Context::Lparen() {
  return getToken(SysYParserParser::Lparen, 0);
}

SysYParserParser::CondContext* SysYParserParser::IfStmt1Context::cond() {
  return getRuleContext<SysYParserParser::CondContext>(0);
}

tree::TerminalNode* SysYParserParser::IfStmt1Context::Rparen() {
  return getToken(SysYParserParser::Rparen, 0);
}

SysYParserParser::StmtContext* SysYParserParser::IfStmt1Context::stmt() {
  return getRuleContext<SysYParserParser::StmtContext>(0);
}

SysYParserParser::IfStmt1Context::IfStmt1Context(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::IfStmt1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitIfStmt1(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BreakStmtContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::BreakStmtContext::Break() {
  return getToken(SysYParserParser::Break, 0);
}

tree::TerminalNode* SysYParserParser::BreakStmtContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

SysYParserParser::BreakStmtContext::BreakStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::BreakStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitBreakStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpStmtContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::ExpStmtContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

SysYParserParser::ExpContext* SysYParserParser::ExpStmtContext::exp() {
  return getRuleContext<SysYParserParser::ExpContext>(0);
}

SysYParserParser::ExpStmtContext::ExpStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ExpStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitExpStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfStmt2Context ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::IfStmt2Context::If() {
  return getToken(SysYParserParser::If, 0);
}

tree::TerminalNode* SysYParserParser::IfStmt2Context::Lparen() {
  return getToken(SysYParserParser::Lparen, 0);
}

SysYParserParser::CondContext* SysYParserParser::IfStmt2Context::cond() {
  return getRuleContext<SysYParserParser::CondContext>(0);
}

tree::TerminalNode* SysYParserParser::IfStmt2Context::Rparen() {
  return getToken(SysYParserParser::Rparen, 0);
}

std::vector<SysYParserParser::StmtContext *> SysYParserParser::IfStmt2Context::stmt() {
  return getRuleContexts<SysYParserParser::StmtContext>();
}

SysYParserParser::StmtContext* SysYParserParser::IfStmt2Context::stmt(size_t i) {
  return getRuleContext<SysYParserParser::StmtContext>(i);
}

tree::TerminalNode* SysYParserParser::IfStmt2Context::Else() {
  return getToken(SysYParserParser::Else, 0);
}

SysYParserParser::IfStmt2Context::IfStmt2Context(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::IfStmt2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitIfStmt2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnStmtContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::ReturnStmtContext::Return() {
  return getToken(SysYParserParser::Return, 0);
}

tree::TerminalNode* SysYParserParser::ReturnStmtContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

SysYParserParser::ExpContext* SysYParserParser::ReturnStmtContext::exp() {
  return getRuleContext<SysYParserParser::ExpContext>(0);
}

SysYParserParser::ReturnStmtContext::ReturnStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ContinueStmtContext ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::ContinueStmtContext::Continue() {
  return getToken(SysYParserParser::Continue, 0);
}

tree::TerminalNode* SysYParserParser::ContinueStmtContext::Semicolon() {
  return getToken(SysYParserParser::Semicolon, 0);
}

SysYParserParser::ContinueStmtContext::ContinueStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ContinueStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitContinueStmt(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::StmtContext* SysYParserParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 30, SysYParserParser::RuleStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(255);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::AssignmentContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(216);
      lVal();
      setState(217);
      match(SysYParserParser::T__0);
      setState(218);
      exp();
      setState(219);
      match(SysYParserParser::Semicolon);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::ExpStmtContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(222);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << SysYParserParser::Lparen)
        | (1ULL << SysYParserParser::Minus)
        | (1ULL << SysYParserParser::Addition)
        | (1ULL << SysYParserParser::Exclamation)
        | (1ULL << SysYParserParser::IntLiteral)
        | (1ULL << SysYParserParser::Identifier))) != 0)) {
        setState(221);
        exp();
      }
      setState(224);
      match(SysYParserParser::Semicolon);
      break;
    }

    case 3: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::BlockStmtContext>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(225);
      block();
      break;
    }

    case 4: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::IfStmt1Context>(_localctx));
      enterOuterAlt(_localctx, 4);
      setState(226);
      match(SysYParserParser::If);
      setState(227);
      match(SysYParserParser::Lparen);
      setState(228);
      cond();
      setState(229);
      match(SysYParserParser::Rparen);
      setState(230);
      stmt();
      break;
    }

    case 5: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::IfStmt2Context>(_localctx));
      enterOuterAlt(_localctx, 5);
      setState(232);
      match(SysYParserParser::If);
      setState(233);
      match(SysYParserParser::Lparen);
      setState(234);
      cond();
      setState(235);
      match(SysYParserParser::Rparen);
      setState(236);
      stmt();
      setState(237);
      match(SysYParserParser::Else);
      setState(238);
      stmt();
      break;
    }

    case 6: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::WhileStmtContext>(_localctx));
      enterOuterAlt(_localctx, 6);
      setState(240);
      match(SysYParserParser::While);
      setState(241);
      match(SysYParserParser::Lparen);
      setState(242);
      cond();
      setState(243);
      match(SysYParserParser::Rparen);
      setState(244);
      stmt();
      break;
    }

    case 7: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::BreakStmtContext>(_localctx));
      enterOuterAlt(_localctx, 7);
      setState(246);
      match(SysYParserParser::Break);
      setState(247);
      match(SysYParserParser::Semicolon);
      break;
    }

    case 8: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::ContinueStmtContext>(_localctx));
      enterOuterAlt(_localctx, 8);
      setState(248);
      match(SysYParserParser::Continue);
      setState(249);
      match(SysYParserParser::Semicolon);
      break;
    }

    case 9: {
      _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<SysYParserParser::ReturnStmtContext>(_localctx));
      enterOuterAlt(_localctx, 9);
      setState(250);
      match(SysYParserParser::Return);
      setState(252);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << SysYParserParser::Lparen)
        | (1ULL << SysYParserParser::Minus)
        | (1ULL << SysYParserParser::Addition)
        | (1ULL << SysYParserParser::Exclamation)
        | (1ULL << SysYParserParser::IntLiteral)
        | (1ULL << SysYParserParser::Identifier))) != 0)) {
        setState(251);
        exp();
      }
      setState(254);
      match(SysYParserParser::Semicolon);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpContext ------------------------------------------------------------------

SysYParserParser::ExpContext::ExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::AddExpContext* SysYParserParser::ExpContext::addExp() {
  return getRuleContext<SysYParserParser::AddExpContext>(0);
}


size_t SysYParserParser::ExpContext::getRuleIndex() const {
  return SysYParserParser::RuleExp;
}


antlrcpp::Any SysYParserParser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::ExpContext* SysYParserParser::exp() {
  ExpContext *_localctx = _tracker.createInstance<ExpContext>(_ctx, getState());
  enterRule(_localctx, 32, SysYParserParser::RuleExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(257);
    addExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondContext ------------------------------------------------------------------

SysYParserParser::CondContext::CondContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::LOrExpContext* SysYParserParser::CondContext::lOrExp() {
  return getRuleContext<SysYParserParser::LOrExpContext>(0);
}


size_t SysYParserParser::CondContext::getRuleIndex() const {
  return SysYParserParser::RuleCond;
}


antlrcpp::Any SysYParserParser::CondContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitCond(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::CondContext* SysYParserParser::cond() {
  CondContext *_localctx = _tracker.createInstance<CondContext>(_ctx, getState());
  enterRule(_localctx, 34, SysYParserParser::RuleCond);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(259);
    lOrExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

SysYParserParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::LValContext::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

std::vector<tree::TerminalNode *> SysYParserParser::LValContext::Lbrkt() {
  return getTokens(SysYParserParser::Lbrkt);
}

tree::TerminalNode* SysYParserParser::LValContext::Lbrkt(size_t i) {
  return getToken(SysYParserParser::Lbrkt, i);
}

std::vector<SysYParserParser::ExpContext *> SysYParserParser::LValContext::exp() {
  return getRuleContexts<SysYParserParser::ExpContext>();
}

SysYParserParser::ExpContext* SysYParserParser::LValContext::exp(size_t i) {
  return getRuleContext<SysYParserParser::ExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::LValContext::Rbrkt() {
  return getTokens(SysYParserParser::Rbrkt);
}

tree::TerminalNode* SysYParserParser::LValContext::Rbrkt(size_t i) {
  return getToken(SysYParserParser::Rbrkt, i);
}


size_t SysYParserParser::LValContext::getRuleIndex() const {
  return SysYParserParser::RuleLVal;
}


antlrcpp::Any SysYParserParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitLVal(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::LValContext* SysYParserParser::lVal() {
  LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
  enterRule(_localctx, 36, SysYParserParser::RuleLVal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(261);
    match(SysYParserParser::Identifier);
    setState(268);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(262);
        match(SysYParserParser::Lbrkt);
        setState(263);
        exp();
        setState(264);
        match(SysYParserParser::Rbrkt); 
      }
      setState(270);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

SysYParserParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::PrimaryExpContext::getRuleIndex() const {
  return SysYParserParser::RulePrimaryExp;
}

void SysYParserParser::PrimaryExpContext::copyFrom(PrimaryExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- PrimaryExp2Context ------------------------------------------------------------------

SysYParserParser::LValContext* SysYParserParser::PrimaryExp2Context::lVal() {
  return getRuleContext<SysYParserParser::LValContext>(0);
}

SysYParserParser::PrimaryExp2Context::PrimaryExp2Context(PrimaryExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::PrimaryExp2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrimaryExp1Context ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::PrimaryExp1Context::Lparen() {
  return getToken(SysYParserParser::Lparen, 0);
}

SysYParserParser::ExpContext* SysYParserParser::PrimaryExp1Context::exp() {
  return getRuleContext<SysYParserParser::ExpContext>(0);
}

tree::TerminalNode* SysYParserParser::PrimaryExp1Context::Rparen() {
  return getToken(SysYParserParser::Rparen, 0);
}

SysYParserParser::PrimaryExp1Context::PrimaryExp1Context(PrimaryExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::PrimaryExp1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp1(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrimaryExp3Context ------------------------------------------------------------------

SysYParserParser::NumberContext* SysYParserParser::PrimaryExp3Context::number() {
  return getRuleContext<SysYParserParser::NumberContext>(0);
}

SysYParserParser::PrimaryExp3Context::PrimaryExp3Context(PrimaryExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::PrimaryExp3Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp3(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::PrimaryExpContext* SysYParserParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 38, SysYParserParser::RulePrimaryExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(277);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParserParser::Lparen: {
        _localctx = dynamic_cast<PrimaryExpContext *>(_tracker.createInstance<SysYParserParser::PrimaryExp1Context>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(271);
        match(SysYParserParser::Lparen);
        setState(272);
        exp();
        setState(273);
        match(SysYParserParser::Rparen);
        break;
      }

      case SysYParserParser::Identifier: {
        _localctx = dynamic_cast<PrimaryExpContext *>(_tracker.createInstance<SysYParserParser::PrimaryExp2Context>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(275);
        lVal();
        break;
      }

      case SysYParserParser::IntLiteral: {
        _localctx = dynamic_cast<PrimaryExpContext *>(_tracker.createInstance<SysYParserParser::PrimaryExp3Context>(_localctx));
        enterOuterAlt(_localctx, 3);
        setState(276);
        number();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

SysYParserParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::NumberContext::IntLiteral() {
  return getToken(SysYParserParser::IntLiteral, 0);
}


size_t SysYParserParser::NumberContext::getRuleIndex() const {
  return SysYParserParser::RuleNumber;
}


antlrcpp::Any SysYParserParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::NumberContext* SysYParserParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 40, SysYParserParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(279);
    match(SysYParserParser::IntLiteral);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

SysYParserParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::UnaryExpContext::getRuleIndex() const {
  return SysYParserParser::RuleUnaryExp;
}

void SysYParserParser::UnaryExpContext::copyFrom(UnaryExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Unary1Context ------------------------------------------------------------------

SysYParserParser::PrimaryExpContext* SysYParserParser::Unary1Context::primaryExp() {
  return getRuleContext<SysYParserParser::PrimaryExpContext>(0);
}

SysYParserParser::Unary1Context::Unary1Context(UnaryExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Unary1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitUnary1(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary2Context ------------------------------------------------------------------

tree::TerminalNode* SysYParserParser::Unary2Context::Identifier() {
  return getToken(SysYParserParser::Identifier, 0);
}

tree::TerminalNode* SysYParserParser::Unary2Context::Lparen() {
  return getToken(SysYParserParser::Lparen, 0);
}

tree::TerminalNode* SysYParserParser::Unary2Context::Rparen() {
  return getToken(SysYParserParser::Rparen, 0);
}

SysYParserParser::FuncRParamsContext* SysYParserParser::Unary2Context::funcRParams() {
  return getRuleContext<SysYParserParser::FuncRParamsContext>(0);
}

SysYParserParser::Unary2Context::Unary2Context(UnaryExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Unary2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitUnary2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary3Context ------------------------------------------------------------------

SysYParserParser::UnaryOpContext* SysYParserParser::Unary3Context::unaryOp() {
  return getRuleContext<SysYParserParser::UnaryOpContext>(0);
}

SysYParserParser::UnaryExpContext* SysYParserParser::Unary3Context::unaryExp() {
  return getRuleContext<SysYParserParser::UnaryExpContext>(0);
}

SysYParserParser::Unary3Context::Unary3Context(UnaryExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Unary3Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitUnary3(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::UnaryExpContext* SysYParserParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 42, SysYParserParser::RuleUnaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(291);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<UnaryExpContext *>(_tracker.createInstance<SysYParserParser::Unary1Context>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(281);
      primaryExp();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<UnaryExpContext *>(_tracker.createInstance<SysYParserParser::Unary2Context>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(282);
      match(SysYParserParser::Identifier);
      setState(283);
      match(SysYParserParser::Lparen);
      setState(285);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << SysYParserParser::Lparen)
        | (1ULL << SysYParserParser::Minus)
        | (1ULL << SysYParserParser::Addition)
        | (1ULL << SysYParserParser::Exclamation)
        | (1ULL << SysYParserParser::IntLiteral)
        | (1ULL << SysYParserParser::Identifier))) != 0)) {
        setState(284);
        funcRParams();
      }
      setState(287);
      match(SysYParserParser::Rparen);
      break;
    }

    case 3: {
      _localctx = dynamic_cast<UnaryExpContext *>(_tracker.createInstance<SysYParserParser::Unary3Context>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(288);
      unaryOp();
      setState(289);
      unaryExp();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOpContext ------------------------------------------------------------------

SysYParserParser::UnaryOpContext::UnaryOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParserParser::UnaryOpContext::Addition() {
  return getToken(SysYParserParser::Addition, 0);
}

tree::TerminalNode* SysYParserParser::UnaryOpContext::Minus() {
  return getToken(SysYParserParser::Minus, 0);
}

tree::TerminalNode* SysYParserParser::UnaryOpContext::Exclamation() {
  return getToken(SysYParserParser::Exclamation, 0);
}


size_t SysYParserParser::UnaryOpContext::getRuleIndex() const {
  return SysYParserParser::RuleUnaryOp;
}


antlrcpp::Any SysYParserParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitUnaryOp(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::UnaryOpContext* SysYParserParser::unaryOp() {
  UnaryOpContext *_localctx = _tracker.createInstance<UnaryOpContext>(_ctx, getState());
  enterRule(_localctx, 44, SysYParserParser::RuleUnaryOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(293);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SysYParserParser::Minus)
      | (1ULL << SysYParserParser::Addition)
      | (1ULL << SysYParserParser::Exclamation))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncRParamsContext ------------------------------------------------------------------

SysYParserParser::FuncRParamsContext::FuncRParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParserParser::FuncRParamContext *> SysYParserParser::FuncRParamsContext::funcRParam() {
  return getRuleContexts<SysYParserParser::FuncRParamContext>();
}

SysYParserParser::FuncRParamContext* SysYParserParser::FuncRParamsContext::funcRParam(size_t i) {
  return getRuleContext<SysYParserParser::FuncRParamContext>(i);
}

std::vector<tree::TerminalNode *> SysYParserParser::FuncRParamsContext::Comma() {
  return getTokens(SysYParserParser::Comma);
}

tree::TerminalNode* SysYParserParser::FuncRParamsContext::Comma(size_t i) {
  return getToken(SysYParserParser::Comma, i);
}


size_t SysYParserParser::FuncRParamsContext::getRuleIndex() const {
  return SysYParserParser::RuleFuncRParams;
}


antlrcpp::Any SysYParserParser::FuncRParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitFuncRParams(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::FuncRParamsContext* SysYParserParser::funcRParams() {
  FuncRParamsContext *_localctx = _tracker.createInstance<FuncRParamsContext>(_ctx, getState());
  enterRule(_localctx, 46, SysYParserParser::RuleFuncRParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(295);
    funcRParam();
    setState(300);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParserParser::Comma) {
      setState(296);
      match(SysYParserParser::Comma);
      setState(297);
      funcRParam();
      setState(302);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncRParamContext ------------------------------------------------------------------

SysYParserParser::FuncRParamContext::FuncRParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::FuncRParamContext::getRuleIndex() const {
  return SysYParserParser::RuleFuncRParam;
}

void SysYParserParser::FuncRParamContext::copyFrom(FuncRParamContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExpAsRParamContext ------------------------------------------------------------------

SysYParserParser::ExpContext* SysYParserParser::ExpAsRParamContext::exp() {
  return getRuleContext<SysYParserParser::ExpContext>(0);
}

SysYParserParser::ExpAsRParamContext::ExpAsRParamContext(FuncRParamContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::ExpAsRParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitExpAsRParam(this);
  else
    return visitor->visitChildren(this);
}
SysYParserParser::FuncRParamContext* SysYParserParser::funcRParam() {
  FuncRParamContext *_localctx = _tracker.createInstance<FuncRParamContext>(_ctx, getState());
  enterRule(_localctx, 48, SysYParserParser::RuleFuncRParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    _localctx = dynamic_cast<FuncRParamContext *>(_tracker.createInstance<SysYParserParser::ExpAsRParamContext>(_localctx));
    enterOuterAlt(_localctx, 1);
    setState(303);
    exp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

SysYParserParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::MulExpContext::getRuleIndex() const {
  return SysYParserParser::RuleMulExp;
}

void SysYParserParser::MulExpContext::copyFrom(MulExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Mul2Context ------------------------------------------------------------------

SysYParserParser::MulExpContext* SysYParserParser::Mul2Context::mulExp() {
  return getRuleContext<SysYParserParser::MulExpContext>(0);
}

SysYParserParser::UnaryExpContext* SysYParserParser::Mul2Context::unaryExp() {
  return getRuleContext<SysYParserParser::UnaryExpContext>(0);
}

tree::TerminalNode* SysYParserParser::Mul2Context::Multiplication() {
  return getToken(SysYParserParser::Multiplication, 0);
}

tree::TerminalNode* SysYParserParser::Mul2Context::Division() {
  return getToken(SysYParserParser::Division, 0);
}

tree::TerminalNode* SysYParserParser::Mul2Context::Modulo() {
  return getToken(SysYParserParser::Modulo, 0);
}

SysYParserParser::Mul2Context::Mul2Context(MulExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Mul2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitMul2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Mul1Context ------------------------------------------------------------------

SysYParserParser::UnaryExpContext* SysYParserParser::Mul1Context::unaryExp() {
  return getRuleContext<SysYParserParser::UnaryExpContext>(0);
}

SysYParserParser::Mul1Context::Mul1Context(MulExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Mul1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitMul1(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::MulExpContext* SysYParserParser::mulExp() {
   return mulExp(0);
}

SysYParserParser::MulExpContext* SysYParserParser::mulExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParserParser::MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, parentState);
  SysYParserParser::MulExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 50;
  enterRecursionRule(_localctx, 50, SysYParserParser::RuleMulExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<Mul1Context>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(306);
    unaryExp();
    _ctx->stop = _input->LT(-1);
    setState(313);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<Mul2Context>(_tracker.createInstance<MulExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleMulExp);
        setState(308);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(309);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SysYParserParser::Multiplication)
          | (1ULL << SysYParserParser::Division)
          | (1ULL << SysYParserParser::Modulo))) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(310);
        unaryExp(); 
      }
      setState(315);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

SysYParserParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::AddExpContext::getRuleIndex() const {
  return SysYParserParser::RuleAddExp;
}

void SysYParserParser::AddExpContext::copyFrom(AddExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Add2Context ------------------------------------------------------------------

SysYParserParser::AddExpContext* SysYParserParser::Add2Context::addExp() {
  return getRuleContext<SysYParserParser::AddExpContext>(0);
}

SysYParserParser::MulExpContext* SysYParserParser::Add2Context::mulExp() {
  return getRuleContext<SysYParserParser::MulExpContext>(0);
}

tree::TerminalNode* SysYParserParser::Add2Context::Addition() {
  return getToken(SysYParserParser::Addition, 0);
}

tree::TerminalNode* SysYParserParser::Add2Context::Minus() {
  return getToken(SysYParserParser::Minus, 0);
}

SysYParserParser::Add2Context::Add2Context(AddExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Add2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitAdd2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Add1Context ------------------------------------------------------------------

SysYParserParser::MulExpContext* SysYParserParser::Add1Context::mulExp() {
  return getRuleContext<SysYParserParser::MulExpContext>(0);
}

SysYParserParser::Add1Context::Add1Context(AddExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Add1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitAdd1(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::AddExpContext* SysYParserParser::addExp() {
   return addExp(0);
}

SysYParserParser::AddExpContext* SysYParserParser::addExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParserParser::AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, parentState);
  SysYParserParser::AddExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 52;
  enterRecursionRule(_localctx, 52, SysYParserParser::RuleAddExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<Add1Context>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(317);
    mulExp(0);
    _ctx->stop = _input->LT(-1);
    setState(324);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<Add2Context>(_tracker.createInstance<AddExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleAddExp);
        setState(319);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(320);
        _la = _input->LA(1);
        if (!(_la == SysYParserParser::Minus

        || _la == SysYParserParser::Addition)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(321);
        mulExp(0); 
      }
      setState(326);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- RelExpContext ------------------------------------------------------------------

SysYParserParser::RelExpContext::RelExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::RelExpContext::getRuleIndex() const {
  return SysYParserParser::RuleRelExp;
}

void SysYParserParser::RelExpContext::copyFrom(RelExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Rel2Context ------------------------------------------------------------------

SysYParserParser::RelExpContext* SysYParserParser::Rel2Context::relExp() {
  return getRuleContext<SysYParserParser::RelExpContext>(0);
}

SysYParserParser::AddExpContext* SysYParserParser::Rel2Context::addExp() {
  return getRuleContext<SysYParserParser::AddExpContext>(0);
}

tree::TerminalNode* SysYParserParser::Rel2Context::LT() {
  return getToken(SysYParserParser::LT, 0);
}

tree::TerminalNode* SysYParserParser::Rel2Context::GT() {
  return getToken(SysYParserParser::GT, 0);
}

tree::TerminalNode* SysYParserParser::Rel2Context::LE() {
  return getToken(SysYParserParser::LE, 0);
}

tree::TerminalNode* SysYParserParser::Rel2Context::GE() {
  return getToken(SysYParserParser::GE, 0);
}

SysYParserParser::Rel2Context::Rel2Context(RelExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Rel2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitRel2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Rel1Context ------------------------------------------------------------------

SysYParserParser::AddExpContext* SysYParserParser::Rel1Context::addExp() {
  return getRuleContext<SysYParserParser::AddExpContext>(0);
}

SysYParserParser::Rel1Context::Rel1Context(RelExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Rel1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitRel1(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::RelExpContext* SysYParserParser::relExp() {
   return relExp(0);
}

SysYParserParser::RelExpContext* SysYParserParser::relExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParserParser::RelExpContext *_localctx = _tracker.createInstance<RelExpContext>(_ctx, parentState);
  SysYParserParser::RelExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 54;
  enterRecursionRule(_localctx, 54, SysYParserParser::RuleRelExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<Rel1Context>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(328);
    addExp(0);
    _ctx->stop = _input->LT(-1);
    setState(335);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<Rel2Context>(_tracker.createInstance<RelExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleRelExp);
        setState(330);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(331);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SysYParserParser::LT)
          | (1ULL << SysYParserParser::LE)
          | (1ULL << SysYParserParser::GT)
          | (1ULL << SysYParserParser::GE))) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(332);
        addExp(0); 
      }
      setState(337);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- EqExpContext ------------------------------------------------------------------

SysYParserParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::EqExpContext::getRuleIndex() const {
  return SysYParserParser::RuleEqExp;
}

void SysYParserParser::EqExpContext::copyFrom(EqExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Eq1Context ------------------------------------------------------------------

SysYParserParser::RelExpContext* SysYParserParser::Eq1Context::relExp() {
  return getRuleContext<SysYParserParser::RelExpContext>(0);
}

SysYParserParser::Eq1Context::Eq1Context(EqExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Eq1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitEq1(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Eq2Context ------------------------------------------------------------------

SysYParserParser::EqExpContext* SysYParserParser::Eq2Context::eqExp() {
  return getRuleContext<SysYParserParser::EqExpContext>(0);
}

SysYParserParser::RelExpContext* SysYParserParser::Eq2Context::relExp() {
  return getRuleContext<SysYParserParser::RelExpContext>(0);
}

tree::TerminalNode* SysYParserParser::Eq2Context::EQ() {
  return getToken(SysYParserParser::EQ, 0);
}

tree::TerminalNode* SysYParserParser::Eq2Context::NEQ() {
  return getToken(SysYParserParser::NEQ, 0);
}

SysYParserParser::Eq2Context::Eq2Context(EqExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::Eq2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitEq2(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::EqExpContext* SysYParserParser::eqExp() {
   return eqExp(0);
}

SysYParserParser::EqExpContext* SysYParserParser::eqExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParserParser::EqExpContext *_localctx = _tracker.createInstance<EqExpContext>(_ctx, parentState);
  SysYParserParser::EqExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 56;
  enterRecursionRule(_localctx, 56, SysYParserParser::RuleEqExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<Eq1Context>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(339);
    relExp(0);
    _ctx->stop = _input->LT(-1);
    setState(346);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<Eq2Context>(_tracker.createInstance<EqExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleEqExp);
        setState(341);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(342);
        _la = _input->LA(1);
        if (!(_la == SysYParserParser::EQ

        || _la == SysYParserParser::NEQ)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(343);
        relExp(0); 
      }
      setState(348);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LAndExpContext ------------------------------------------------------------------

SysYParserParser::LAndExpContext::LAndExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::LAndExpContext::getRuleIndex() const {
  return SysYParserParser::RuleLAndExp;
}

void SysYParserParser::LAndExpContext::copyFrom(LAndExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LAnd2Context ------------------------------------------------------------------

SysYParserParser::LAndExpContext* SysYParserParser::LAnd2Context::lAndExp() {
  return getRuleContext<SysYParserParser::LAndExpContext>(0);
}

tree::TerminalNode* SysYParserParser::LAnd2Context::LAND() {
  return getToken(SysYParserParser::LAND, 0);
}

SysYParserParser::EqExpContext* SysYParserParser::LAnd2Context::eqExp() {
  return getRuleContext<SysYParserParser::EqExpContext>(0);
}

SysYParserParser::LAnd2Context::LAnd2Context(LAndExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::LAnd2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitLAnd2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LAnd1Context ------------------------------------------------------------------

SysYParserParser::EqExpContext* SysYParserParser::LAnd1Context::eqExp() {
  return getRuleContext<SysYParserParser::EqExpContext>(0);
}

SysYParserParser::LAnd1Context::LAnd1Context(LAndExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::LAnd1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitLAnd1(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::LAndExpContext* SysYParserParser::lAndExp() {
   return lAndExp(0);
}

SysYParserParser::LAndExpContext* SysYParserParser::lAndExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParserParser::LAndExpContext *_localctx = _tracker.createInstance<LAndExpContext>(_ctx, parentState);
  SysYParserParser::LAndExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 58;
  enterRecursionRule(_localctx, 58, SysYParserParser::RuleLAndExp, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<LAnd1Context>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(350);
    eqExp(0);
    _ctx->stop = _input->LT(-1);
    setState(357);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<LAnd2Context>(_tracker.createInstance<LAndExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleLAndExp);
        setState(352);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(353);
        match(SysYParserParser::LAND);
        setState(354);
        eqExp(0); 
      }
      setState(359);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LOrExpContext ------------------------------------------------------------------

SysYParserParser::LOrExpContext::LOrExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParserParser::LOrExpContext::getRuleIndex() const {
  return SysYParserParser::RuleLOrExp;
}

void SysYParserParser::LOrExpContext::copyFrom(LOrExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LOr1Context ------------------------------------------------------------------

SysYParserParser::LAndExpContext* SysYParserParser::LOr1Context::lAndExp() {
  return getRuleContext<SysYParserParser::LAndExpContext>(0);
}

SysYParserParser::LOr1Context::LOr1Context(LOrExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::LOr1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitLOr1(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LOr2Context ------------------------------------------------------------------

SysYParserParser::LOrExpContext* SysYParserParser::LOr2Context::lOrExp() {
  return getRuleContext<SysYParserParser::LOrExpContext>(0);
}

tree::TerminalNode* SysYParserParser::LOr2Context::LOR() {
  return getToken(SysYParserParser::LOR, 0);
}

SysYParserParser::LAndExpContext* SysYParserParser::LOr2Context::lAndExp() {
  return getRuleContext<SysYParserParser::LAndExpContext>(0);
}

SysYParserParser::LOr2Context::LOr2Context(LOrExpContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SysYParserParser::LOr2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitLOr2(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::LOrExpContext* SysYParserParser::lOrExp() {
   return lOrExp(0);
}

SysYParserParser::LOrExpContext* SysYParserParser::lOrExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParserParser::LOrExpContext *_localctx = _tracker.createInstance<LOrExpContext>(_ctx, parentState);
  SysYParserParser::LOrExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 60;
  enterRecursionRule(_localctx, 60, SysYParserParser::RuleLOrExp, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<LOr1Context>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(361);
    lAndExp(0);
    _ctx->stop = _input->LT(-1);
    setState(368);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<LOr2Context>(_tracker.createInstance<LOrExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleLOrExp);
        setState(363);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(364);
        match(SysYParserParser::LOR);
        setState(365);
        lAndExp(0); 
      }
      setState(370);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ConstExpContext ------------------------------------------------------------------

SysYParserParser::ConstExpContext::ConstExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParserParser::AddExpContext* SysYParserParser::ConstExpContext::addExp() {
  return getRuleContext<SysYParserParser::AddExpContext>(0);
}


size_t SysYParserParser::ConstExpContext::getRuleIndex() const {
  return SysYParserParser::RuleConstExp;
}


antlrcpp::Any SysYParserParser::ConstExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYParserVisitor*>(visitor))
    return parserVisitor->visitConstExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParserParser::ConstExpContext* SysYParserParser::constExp() {
  ConstExpContext *_localctx = _tracker.createInstance<ConstExpContext>(_ctx, getState());
  enterRule(_localctx, 62, SysYParserParser::RuleConstExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(371);
    addExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool SysYParserParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 25: return mulExpSempred(dynamic_cast<MulExpContext *>(context), predicateIndex);
    case 26: return addExpSempred(dynamic_cast<AddExpContext *>(context), predicateIndex);
    case 27: return relExpSempred(dynamic_cast<RelExpContext *>(context), predicateIndex);
    case 28: return eqExpSempred(dynamic_cast<EqExpContext *>(context), predicateIndex);
    case 29: return lAndExpSempred(dynamic_cast<LAndExpContext *>(context), predicateIndex);
    case 30: return lOrExpSempred(dynamic_cast<LOrExpContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SysYParserParser::mulExpSempred(MulExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParserParser::addExpSempred(AddExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParserParser::relExpSempred(RelExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParserParser::eqExpSempred(EqExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 3: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParserParser::lAndExpSempred(LAndExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParserParser::lOrExpSempred(LOrExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> SysYParserParser::_decisionToDFA;
atn::PredictionContextCache SysYParserParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SysYParserParser::_atn;
std::vector<uint16_t> SysYParserParser::_serializedATN;

std::vector<std::string> SysYParserParser::_ruleNames = {
  "compUnit", "decl", "constDecl", "bType", "constDef", "constInitVal", 
  "varDecl", "varDef", "initVal", "funcDef", "funcType", "funcFParams", 
  "funcFParam", "block", "blockItem", "stmt", "exp", "cond", "lVal", "primaryExp", 
  "number", "unaryExp", "unaryOp", "funcRParams", "funcRParam", "mulExp", 
  "addExp", "relExp", "eqExp", "lAndExp", "lOrExp", "constExp"
};

std::vector<std::string> SysYParserParser::_literalNames = {
  "", "'='", "'int'", "'void'", "'const'", "'return'", "'if'", "'else'", 
  "'while'", "'break'", "'continue'", "'('", "')'", "'['", "']'", "'{'", 
  "'}'", "','", "';'", "'-'", "'+'", "'!'", "'*'", "'/'", "'%'", "'&&'", 
  "'||'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='"
};

std::vector<std::string> SysYParserParser::_symbolicNames = {
  "", "", "Int", "Void", "Const", "Return", "If", "Else", "While", "Break", 
  "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", "Rbrace", 
  "Comma", "Semicolon", "Minus", "Addition", "Exclamation", "Multiplication", 
  "Division", "Modulo", "LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", 
  "IntLiteral", "Identifier", "WS", "LINE_COMMENT", "COMMENT"
};

dfa::Vocabulary SysYParserParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SysYParserParser::_tokenNames;

SysYParserParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x27, 0x178, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 
       0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 
       0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 
       0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x4, 0x20, 
       0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x45, 
       0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x48, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 
       0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x4e, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 
       0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x55, 0xa, 0x4, 0xc, 0x4, 
       0xe, 0x4, 0x58, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x63, 
       0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0x66, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 
       0x7, 0x70, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x73, 0xb, 0x7, 0x5, 0x7, 
       0x75, 0xa, 0x7, 0x3, 0x7, 0x5, 0x7, 0x78, 0xa, 0x7, 0x3, 0x8, 0x3, 
       0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x7e, 0xa, 0x8, 0xc, 0x8, 0xe, 
       0x8, 0x81, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x8a, 0xa, 0x9, 0xc, 0x9, 0xe, 
       0x9, 0x8d, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x7, 0x9, 0x94, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x97, 0xb, 0x9, 
       0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x9b, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 
       0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 0xa2, 0xa, 0xa, 0xc, 0xa, 
       0xe, 0xa, 0xa5, 0xb, 0xa, 0x5, 0xa, 0xa7, 0xa, 0xa, 0x3, 0xa, 0x5, 
       0xa, 0xaa, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 
       0xb, 0xb0, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 
       0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xba, 0xa, 0xd, 0xc, 
       0xd, 0xe, 0xd, 0xbd, 0xb, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
       0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0xc7, 0xa, 
       0xe, 0xc, 0xe, 0xe, 0xe, 0xca, 0xb, 0xe, 0x5, 0xe, 0xcc, 0xa, 0xe, 
       0x3, 0xf, 0x3, 0xf, 0x7, 0xf, 0xd0, 0xa, 0xf, 0xc, 0xf, 0xe, 0xf, 
       0xd3, 0xb, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 
       0xd9, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 
       0x11, 0x3, 0x11, 0x5, 0x11, 0xe1, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0xff, 0xa, 0x11, 0x3, 0x11, 0x5, 
       0x11, 0x102, 0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 
       0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 
       0x10d, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0x110, 0xb, 0x14, 0x3, 0x15, 
       0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 
       0x118, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 
       0x17, 0x3, 0x17, 0x5, 0x17, 0x120, 0xa, 0x17, 0x3, 0x17, 0x3, 0x17, 
       0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x126, 0xa, 0x17, 0x3, 0x18, 0x3, 
       0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x12d, 0xa, 0x19, 
       0xc, 0x19, 0xe, 0x19, 0x130, 0xb, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
       0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 
       0x1b, 0x13a, 0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 0x13d, 0xb, 0x1b, 0x3, 
       0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x7, 
       0x1c, 0x145, 0xa, 0x1c, 0xc, 0x1c, 0xe, 0x1c, 0x148, 0xb, 0x1c, 0x3, 
       0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 
       0x1d, 0x150, 0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x153, 0xb, 0x1d, 0x3, 
       0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x7, 
       0x1e, 0x15b, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x15e, 0xb, 0x1e, 0x3, 
       0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x7, 
       0x1f, 0x166, 0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 0x169, 0xb, 0x1f, 0x3, 
       0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x7, 
       0x20, 0x171, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x174, 0xb, 0x20, 0x3, 
       0x21, 0x3, 0x21, 0x3, 0x21, 0x2, 0x8, 0x34, 0x36, 0x38, 0x3a, 0x3c, 
       0x3e, 0x22, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 
       0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 
       0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 
       0x2, 0x8, 0x3, 0x2, 0x4, 0x5, 0x3, 0x2, 0x15, 0x17, 0x3, 0x2, 0x18, 
       0x1a, 0x3, 0x2, 0x15, 0x16, 0x3, 0x2, 0x1f, 0x22, 0x3, 0x2, 0x1d, 
       0x1e, 0x2, 0x183, 0x2, 0x46, 0x3, 0x2, 0x2, 0x2, 0x4, 0x4d, 0x3, 
       0x2, 0x2, 0x2, 0x6, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x8, 0x5b, 0x3, 0x2, 
       0x2, 0x2, 0xa, 0x5d, 0x3, 0x2, 0x2, 0x2, 0xc, 0x77, 0x3, 0x2, 0x2, 
       0x2, 0xe, 0x79, 0x3, 0x2, 0x2, 0x2, 0x10, 0x9a, 0x3, 0x2, 0x2, 0x2, 
       0x12, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x14, 0xab, 0x3, 0x2, 0x2, 0x2, 0x16, 
       0xb4, 0x3, 0x2, 0x2, 0x2, 0x18, 0xb6, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xbe, 
       0x3, 0x2, 0x2, 0x2, 0x1c, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xd8, 0x3, 
       0x2, 0x2, 0x2, 0x20, 0x101, 0x3, 0x2, 0x2, 0x2, 0x22, 0x103, 0x3, 
       0x2, 0x2, 0x2, 0x24, 0x105, 0x3, 0x2, 0x2, 0x2, 0x26, 0x107, 0x3, 
       0x2, 0x2, 0x2, 0x28, 0x117, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x119, 0x3, 
       0x2, 0x2, 0x2, 0x2c, 0x125, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x127, 0x3, 
       0x2, 0x2, 0x2, 0x30, 0x129, 0x3, 0x2, 0x2, 0x2, 0x32, 0x131, 0x3, 
       0x2, 0x2, 0x2, 0x34, 0x133, 0x3, 0x2, 0x2, 0x2, 0x36, 0x13e, 0x3, 
       0x2, 0x2, 0x2, 0x38, 0x149, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x154, 0x3, 
       0x2, 0x2, 0x2, 0x3c, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x16a, 0x3, 
       0x2, 0x2, 0x2, 0x40, 0x175, 0x3, 0x2, 0x2, 0x2, 0x42, 0x45, 0x5, 
       0x4, 0x3, 0x2, 0x43, 0x45, 0x5, 0x14, 0xb, 0x2, 0x44, 0x42, 0x3, 
       0x2, 0x2, 0x2, 0x44, 0x43, 0x3, 0x2, 0x2, 0x2, 0x45, 0x48, 0x3, 0x2, 
       0x2, 0x2, 0x46, 0x44, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 
       0x2, 0x47, 0x49, 0x3, 0x2, 0x2, 0x2, 0x48, 0x46, 0x3, 0x2, 0x2, 0x2, 
       0x49, 0x4a, 0x7, 0x2, 0x2, 0x3, 0x4a, 0x3, 0x3, 0x2, 0x2, 0x2, 0x4b, 
       0x4e, 0x5, 0x6, 0x4, 0x2, 0x4c, 0x4e, 0x5, 0xe, 0x8, 0x2, 0x4d, 0x4b, 
       0x3, 0x2, 0x2, 0x2, 0x4d, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x5, 0x3, 
       0x2, 0x2, 0x2, 0x4f, 0x50, 0x7, 0x6, 0x2, 0x2, 0x50, 0x51, 0x5, 0x8, 
       0x5, 0x2, 0x51, 0x56, 0x5, 0xa, 0x6, 0x2, 0x52, 0x53, 0x7, 0x13, 
       0x2, 0x2, 0x53, 0x55, 0x5, 0xa, 0x6, 0x2, 0x54, 0x52, 0x3, 0x2, 0x2, 
       0x2, 0x55, 0x58, 0x3, 0x2, 0x2, 0x2, 0x56, 0x54, 0x3, 0x2, 0x2, 0x2, 
       0x56, 0x57, 0x3, 0x2, 0x2, 0x2, 0x57, 0x59, 0x3, 0x2, 0x2, 0x2, 0x58, 
       0x56, 0x3, 0x2, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x14, 0x2, 0x2, 0x5a, 
       0x7, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5c, 0x7, 0x4, 0x2, 0x2, 0x5c, 0x9, 
       0x3, 0x2, 0x2, 0x2, 0x5d, 0x64, 0x7, 0x24, 0x2, 0x2, 0x5e, 0x5f, 
       0x7, 0xf, 0x2, 0x2, 0x5f, 0x60, 0x5, 0x40, 0x21, 0x2, 0x60, 0x61, 
       0x7, 0x10, 0x2, 0x2, 0x61, 0x63, 0x3, 0x2, 0x2, 0x2, 0x62, 0x5e, 
       0x3, 0x2, 0x2, 0x2, 0x63, 0x66, 0x3, 0x2, 0x2, 0x2, 0x64, 0x62, 0x3, 
       0x2, 0x2, 0x2, 0x64, 0x65, 0x3, 0x2, 0x2, 0x2, 0x65, 0x67, 0x3, 0x2, 
       0x2, 0x2, 0x66, 0x64, 0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 0x7, 0x3, 0x2, 
       0x2, 0x68, 0x69, 0x5, 0xc, 0x7, 0x2, 0x69, 0xb, 0x3, 0x2, 0x2, 0x2, 
       0x6a, 0x78, 0x5, 0x40, 0x21, 0x2, 0x6b, 0x74, 0x7, 0x11, 0x2, 0x2, 
       0x6c, 0x71, 0x5, 0xc, 0x7, 0x2, 0x6d, 0x6e, 0x7, 0x13, 0x2, 0x2, 
       0x6e, 0x70, 0x5, 0xc, 0x7, 0x2, 0x6f, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x70, 
       0x73, 0x3, 0x2, 0x2, 0x2, 0x71, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x71, 0x72, 
       0x3, 0x2, 0x2, 0x2, 0x72, 0x75, 0x3, 0x2, 0x2, 0x2, 0x73, 0x71, 0x3, 
       0x2, 0x2, 0x2, 0x74, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x3, 0x2, 
       0x2, 0x2, 0x75, 0x76, 0x3, 0x2, 0x2, 0x2, 0x76, 0x78, 0x7, 0x12, 
       0x2, 0x2, 0x77, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x77, 0x6b, 0x3, 0x2, 0x2, 
       0x2, 0x78, 0xd, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x5, 0x8, 0x5, 0x2, 
       0x7a, 0x7f, 0x5, 0x10, 0x9, 0x2, 0x7b, 0x7c, 0x7, 0x13, 0x2, 0x2, 
       0x7c, 0x7e, 0x5, 0x10, 0x9, 0x2, 0x7d, 0x7b, 0x3, 0x2, 0x2, 0x2, 
       0x7e, 0x81, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x7f, 
       0x80, 0x3, 0x2, 0x2, 0x2, 0x80, 0x82, 0x3, 0x2, 0x2, 0x2, 0x81, 0x7f, 
       0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x7, 0x14, 0x2, 0x2, 0x83, 0xf, 0x3, 
       0x2, 0x2, 0x2, 0x84, 0x8b, 0x7, 0x24, 0x2, 0x2, 0x85, 0x86, 0x7, 
       0xf, 0x2, 0x2, 0x86, 0x87, 0x5, 0x40, 0x21, 0x2, 0x87, 0x88, 0x7, 
       0x10, 0x2, 0x2, 0x88, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x89, 0x85, 0x3, 
       0x2, 0x2, 0x2, 0x8a, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x89, 0x3, 0x2, 
       0x2, 0x2, 0x8b, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x9b, 0x3, 0x2, 0x2, 
       0x2, 0x8d, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x95, 0x7, 0x24, 0x2, 
       0x2, 0x8f, 0x90, 0x7, 0xf, 0x2, 0x2, 0x90, 0x91, 0x5, 0x40, 0x21, 
       0x2, 0x91, 0x92, 0x7, 0x10, 0x2, 0x2, 0x92, 0x94, 0x3, 0x2, 0x2, 
       0x2, 0x93, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x94, 0x97, 0x3, 0x2, 0x2, 0x2, 
       0x95, 0x93, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x3, 0x2, 0x2, 0x2, 0x96, 
       0x98, 0x3, 0x2, 0x2, 0x2, 0x97, 0x95, 0x3, 0x2, 0x2, 0x2, 0x98, 0x99, 
       0x7, 0x3, 0x2, 0x2, 0x99, 0x9b, 0x5, 0x12, 0xa, 0x2, 0x9a, 0x84, 
       0x3, 0x2, 0x2, 0x2, 0x9a, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x11, 0x3, 
       0x2, 0x2, 0x2, 0x9c, 0xaa, 0x5, 0x22, 0x12, 0x2, 0x9d, 0xa6, 0x7, 
       0x11, 0x2, 0x2, 0x9e, 0xa3, 0x5, 0x12, 0xa, 0x2, 0x9f, 0xa0, 0x7, 
       0x13, 0x2, 0x2, 0xa0, 0xa2, 0x5, 0x12, 0xa, 0x2, 0xa1, 0x9f, 0x3, 
       0x2, 0x2, 0x2, 0xa2, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa1, 0x3, 0x2, 
       0x2, 0x2, 0xa3, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa7, 0x3, 0x2, 0x2, 
       0x2, 0xa5, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x9e, 0x3, 0x2, 0x2, 0x2, 
       0xa6, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa8, 
       0xaa, 0x7, 0x12, 0x2, 0x2, 0xa9, 0x9c, 0x3, 0x2, 0x2, 0x2, 0xa9, 
       0x9d, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x13, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 
       0x5, 0x16, 0xc, 0x2, 0xac, 0xad, 0x7, 0x24, 0x2, 0x2, 0xad, 0xaf, 
       0x7, 0xd, 0x2, 0x2, 0xae, 0xb0, 0x5, 0x18, 0xd, 0x2, 0xaf, 0xae, 
       0x3, 0x2, 0x2, 0x2, 0xaf, 0xb0, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb1, 0x3, 
       0x2, 0x2, 0x2, 0xb1, 0xb2, 0x7, 0xe, 0x2, 0x2, 0xb2, 0xb3, 0x5, 0x1c, 
       0xf, 0x2, 0xb3, 0x15, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb5, 0x9, 0x2, 0x2, 
       0x2, 0xb5, 0x17, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xbb, 0x5, 0x1a, 0xe, 
       0x2, 0xb7, 0xb8, 0x7, 0x13, 0x2, 0x2, 0xb8, 0xba, 0x5, 0x1a, 0xe, 
       0x2, 0xb9, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbd, 0x3, 0x2, 0x2, 0x2, 
       0xbb, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xbc, 
       0x19, 0x3, 0x2, 0x2, 0x2, 0xbd, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xbf, 
       0x5, 0x8, 0x5, 0x2, 0xbf, 0xcb, 0x7, 0x24, 0x2, 0x2, 0xc0, 0xc1, 
       0x7, 0xf, 0x2, 0x2, 0xc1, 0xc8, 0x7, 0x10, 0x2, 0x2, 0xc2, 0xc3, 
       0x7, 0xf, 0x2, 0x2, 0xc3, 0xc4, 0x5, 0x40, 0x21, 0x2, 0xc4, 0xc5, 
       0x7, 0x10, 0x2, 0x2, 0xc5, 0xc7, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc2, 
       0x3, 0x2, 0x2, 0x2, 0xc7, 0xca, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc6, 0x3, 
       0x2, 0x2, 0x2, 0xc8, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xc9, 0xcc, 0x3, 0x2, 
       0x2, 0x2, 0xca, 0xc8, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xc0, 0x3, 0x2, 0x2, 
       0x2, 0xcb, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xcc, 0x1b, 0x3, 0x2, 0x2, 0x2, 
       0xcd, 0xd1, 0x7, 0x11, 0x2, 0x2, 0xce, 0xd0, 0x5, 0x1e, 0x10, 0x2, 
       0xcf, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd1, 
       0xcf, 0x3, 0x2, 0x2, 0x2, 0xd1, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd4, 
       0x3, 0x2, 0x2, 0x2, 0xd3, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd5, 0x7, 
       0x12, 0x2, 0x2, 0xd5, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd9, 0x5, 
       0x4, 0x3, 0x2, 0xd7, 0xd9, 0x5, 0x20, 0x11, 0x2, 0xd8, 0xd6, 0x3, 
       0x2, 0x2, 0x2, 0xd8, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xd9, 0x1f, 0x3, 0x2, 
       0x2, 0x2, 0xda, 0xdb, 0x5, 0x26, 0x14, 0x2, 0xdb, 0xdc, 0x7, 0x3, 
       0x2, 0x2, 0xdc, 0xdd, 0x5, 0x22, 0x12, 0x2, 0xdd, 0xde, 0x7, 0x14, 
       0x2, 0x2, 0xde, 0x102, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe1, 0x5, 0x22, 
       0x12, 0x2, 0xe0, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x3, 0x2, 
       0x2, 0x2, 0xe1, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe2, 0x102, 0x7, 0x14, 
       0x2, 0x2, 0xe3, 0x102, 0x5, 0x1c, 0xf, 0x2, 0xe4, 0xe5, 0x7, 0x8, 
       0x2, 0x2, 0xe5, 0xe6, 0x7, 0xd, 0x2, 0x2, 0xe6, 0xe7, 0x5, 0x24, 
       0x13, 0x2, 0xe7, 0xe8, 0x7, 0xe, 0x2, 0x2, 0xe8, 0xe9, 0x5, 0x20, 
       0x11, 0x2, 0xe9, 0x102, 0x3, 0x2, 0x2, 0x2, 0xea, 0xeb, 0x7, 0x8, 
       0x2, 0x2, 0xeb, 0xec, 0x7, 0xd, 0x2, 0x2, 0xec, 0xed, 0x5, 0x24, 
       0x13, 0x2, 0xed, 0xee, 0x7, 0xe, 0x2, 0x2, 0xee, 0xef, 0x5, 0x20, 
       0x11, 0x2, 0xef, 0xf0, 0x7, 0x9, 0x2, 0x2, 0xf0, 0xf1, 0x5, 0x20, 
       0x11, 0x2, 0xf1, 0x102, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf3, 0x7, 0xa, 
       0x2, 0x2, 0xf3, 0xf4, 0x7, 0xd, 0x2, 0x2, 0xf4, 0xf5, 0x5, 0x24, 
       0x13, 0x2, 0xf5, 0xf6, 0x7, 0xe, 0x2, 0x2, 0xf6, 0xf7, 0x5, 0x20, 
       0x11, 0x2, 0xf7, 0x102, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xf9, 0x7, 0xb, 
       0x2, 0x2, 0xf9, 0x102, 0x7, 0x14, 0x2, 0x2, 0xfa, 0xfb, 0x7, 0xc, 
       0x2, 0x2, 0xfb, 0x102, 0x7, 0x14, 0x2, 0x2, 0xfc, 0xfe, 0x7, 0x7, 
       0x2, 0x2, 0xfd, 0xff, 0x5, 0x22, 0x12, 0x2, 0xfe, 0xfd, 0x3, 0x2, 
       0x2, 0x2, 0xfe, 0xff, 0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 0x3, 0x2, 
       0x2, 0x2, 0x100, 0x102, 0x7, 0x14, 0x2, 0x2, 0x101, 0xda, 0x3, 0x2, 
       0x2, 0x2, 0x101, 0xe0, 0x3, 0x2, 0x2, 0x2, 0x101, 0xe3, 0x3, 0x2, 
       0x2, 0x2, 0x101, 0xe4, 0x3, 0x2, 0x2, 0x2, 0x101, 0xea, 0x3, 0x2, 
       0x2, 0x2, 0x101, 0xf2, 0x3, 0x2, 0x2, 0x2, 0x101, 0xf8, 0x3, 0x2, 
       0x2, 0x2, 0x101, 0xfa, 0x3, 0x2, 0x2, 0x2, 0x101, 0xfc, 0x3, 0x2, 
       0x2, 0x2, 0x102, 0x21, 0x3, 0x2, 0x2, 0x2, 0x103, 0x104, 0x5, 0x36, 
       0x1c, 0x2, 0x104, 0x23, 0x3, 0x2, 0x2, 0x2, 0x105, 0x106, 0x5, 0x3e, 
       0x20, 0x2, 0x106, 0x25, 0x3, 0x2, 0x2, 0x2, 0x107, 0x10e, 0x7, 0x24, 
       0x2, 0x2, 0x108, 0x109, 0x7, 0xf, 0x2, 0x2, 0x109, 0x10a, 0x5, 0x22, 
       0x12, 0x2, 0x10a, 0x10b, 0x7, 0x10, 0x2, 0x2, 0x10b, 0x10d, 0x3, 
       0x2, 0x2, 0x2, 0x10c, 0x108, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x110, 0x3, 
       0x2, 0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10f, 0x3, 
       0x2, 0x2, 0x2, 0x10f, 0x27, 0x3, 0x2, 0x2, 0x2, 0x110, 0x10e, 0x3, 
       0x2, 0x2, 0x2, 0x111, 0x112, 0x7, 0xd, 0x2, 0x2, 0x112, 0x113, 0x5, 
       0x22, 0x12, 0x2, 0x113, 0x114, 0x7, 0xe, 0x2, 0x2, 0x114, 0x118, 
       0x3, 0x2, 0x2, 0x2, 0x115, 0x118, 0x5, 0x26, 0x14, 0x2, 0x116, 0x118, 
       0x5, 0x2a, 0x16, 0x2, 0x117, 0x111, 0x3, 0x2, 0x2, 0x2, 0x117, 0x115, 
       0x3, 0x2, 0x2, 0x2, 0x117, 0x116, 0x3, 0x2, 0x2, 0x2, 0x118, 0x29, 
       0x3, 0x2, 0x2, 0x2, 0x119, 0x11a, 0x7, 0x23, 0x2, 0x2, 0x11a, 0x2b, 
       0x3, 0x2, 0x2, 0x2, 0x11b, 0x126, 0x5, 0x28, 0x15, 0x2, 0x11c, 0x11d, 
       0x7, 0x24, 0x2, 0x2, 0x11d, 0x11f, 0x7, 0xd, 0x2, 0x2, 0x11e, 0x120, 
       0x5, 0x30, 0x19, 0x2, 0x11f, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x120, 
       0x3, 0x2, 0x2, 0x2, 0x120, 0x121, 0x3, 0x2, 0x2, 0x2, 0x121, 0x126, 
       0x7, 0xe, 0x2, 0x2, 0x122, 0x123, 0x5, 0x2e, 0x18, 0x2, 0x123, 0x124, 
       0x5, 0x2c, 0x17, 0x2, 0x124, 0x126, 0x3, 0x2, 0x2, 0x2, 0x125, 0x11b, 
       0x3, 0x2, 0x2, 0x2, 0x125, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x125, 0x122, 
       0x3, 0x2, 0x2, 0x2, 0x126, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x127, 0x128, 
       0x9, 0x3, 0x2, 0x2, 0x128, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x129, 0x12e, 
       0x5, 0x32, 0x1a, 0x2, 0x12a, 0x12b, 0x7, 0x13, 0x2, 0x2, 0x12b, 0x12d, 
       0x5, 0x32, 0x1a, 0x2, 0x12c, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x130, 
       0x3, 0x2, 0x2, 0x2, 0x12e, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x12f, 
       0x3, 0x2, 0x2, 0x2, 0x12f, 0x31, 0x3, 0x2, 0x2, 0x2, 0x130, 0x12e, 
       0x3, 0x2, 0x2, 0x2, 0x131, 0x132, 0x5, 0x22, 0x12, 0x2, 0x132, 0x33, 
       0x3, 0x2, 0x2, 0x2, 0x133, 0x134, 0x8, 0x1b, 0x1, 0x2, 0x134, 0x135, 
       0x5, 0x2c, 0x17, 0x2, 0x135, 0x13b, 0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 
       0xc, 0x3, 0x2, 0x2, 0x137, 0x138, 0x9, 0x4, 0x2, 0x2, 0x138, 0x13a, 
       0x5, 0x2c, 0x17, 0x2, 0x139, 0x136, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13d, 
       0x3, 0x2, 0x2, 0x2, 0x13b, 0x139, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x13c, 
       0x3, 0x2, 0x2, 0x2, 0x13c, 0x35, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x13b, 
       0x3, 0x2, 0x2, 0x2, 0x13e, 0x13f, 0x8, 0x1c, 0x1, 0x2, 0x13f, 0x140, 
       0x5, 0x34, 0x1b, 0x2, 0x140, 0x146, 0x3, 0x2, 0x2, 0x2, 0x141, 0x142, 
       0xc, 0x3, 0x2, 0x2, 0x142, 0x143, 0x9, 0x5, 0x2, 0x2, 0x143, 0x145, 
       0x5, 0x34, 0x1b, 0x2, 0x144, 0x141, 0x3, 0x2, 0x2, 0x2, 0x145, 0x148, 
       0x3, 0x2, 0x2, 0x2, 0x146, 0x144, 0x3, 0x2, 0x2, 0x2, 0x146, 0x147, 
       0x3, 0x2, 0x2, 0x2, 0x147, 0x37, 0x3, 0x2, 0x2, 0x2, 0x148, 0x146, 
       0x3, 0x2, 0x2, 0x2, 0x149, 0x14a, 0x8, 0x1d, 0x1, 0x2, 0x14a, 0x14b, 
       0x5, 0x36, 0x1c, 0x2, 0x14b, 0x151, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x14d, 
       0xc, 0x3, 0x2, 0x2, 0x14d, 0x14e, 0x9, 0x6, 0x2, 0x2, 0x14e, 0x150, 
       0x5, 0x36, 0x1c, 0x2, 0x14f, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x150, 0x153, 
       0x3, 0x2, 0x2, 0x2, 0x151, 0x14f, 0x3, 0x2, 0x2, 0x2, 0x151, 0x152, 
       0x3, 0x2, 0x2, 0x2, 0x152, 0x39, 0x3, 0x2, 0x2, 0x2, 0x153, 0x151, 
       0x3, 0x2, 0x2, 0x2, 0x154, 0x155, 0x8, 0x1e, 0x1, 0x2, 0x155, 0x156, 
       0x5, 0x38, 0x1d, 0x2, 0x156, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x157, 0x158, 
       0xc, 0x3, 0x2, 0x2, 0x158, 0x159, 0x9, 0x7, 0x2, 0x2, 0x159, 0x15b, 
       0x5, 0x38, 0x1d, 0x2, 0x15a, 0x157, 0x3, 0x2, 0x2, 0x2, 0x15b, 0x15e, 
       0x3, 0x2, 0x2, 0x2, 0x15c, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15d, 
       0x3, 0x2, 0x2, 0x2, 0x15d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x15c, 
       0x3, 0x2, 0x2, 0x2, 0x15f, 0x160, 0x8, 0x1f, 0x1, 0x2, 0x160, 0x161, 
       0x5, 0x3a, 0x1e, 0x2, 0x161, 0x167, 0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 
       0xc, 0x3, 0x2, 0x2, 0x163, 0x164, 0x7, 0x1b, 0x2, 0x2, 0x164, 0x166, 
       0x5, 0x3a, 0x1e, 0x2, 0x165, 0x162, 0x3, 0x2, 0x2, 0x2, 0x166, 0x169, 
       0x3, 0x2, 0x2, 0x2, 0x167, 0x165, 0x3, 0x2, 0x2, 0x2, 0x167, 0x168, 
       0x3, 0x2, 0x2, 0x2, 0x168, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x169, 0x167, 
       0x3, 0x2, 0x2, 0x2, 0x16a, 0x16b, 0x8, 0x20, 0x1, 0x2, 0x16b, 0x16c, 
       0x5, 0x3c, 0x1f, 0x2, 0x16c, 0x172, 0x3, 0x2, 0x2, 0x2, 0x16d, 0x16e, 
       0xc, 0x3, 0x2, 0x2, 0x16e, 0x16f, 0x7, 0x1c, 0x2, 0x2, 0x16f, 0x171, 
       0x5, 0x3c, 0x1f, 0x2, 0x170, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x171, 0x174, 
       0x3, 0x2, 0x2, 0x2, 0x172, 0x170, 0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 
       0x3, 0x2, 0x2, 0x2, 0x173, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x174, 0x172, 
       0x3, 0x2, 0x2, 0x2, 0x175, 0x176, 0x5, 0x36, 0x1c, 0x2, 0x176, 0x41, 
       0x3, 0x2, 0x2, 0x2, 0x25, 0x44, 0x46, 0x4d, 0x56, 0x64, 0x71, 0x74, 
       0x77, 0x7f, 0x8b, 0x95, 0x9a, 0xa3, 0xa6, 0xa9, 0xaf, 0xbb, 0xc8, 
       0xcb, 0xd1, 0xd8, 0xe0, 0xfe, 0x101, 0x10e, 0x117, 0x11f, 0x125, 
       0x12e, 0x13b, 0x146, 0x151, 0x15c, 0x167, 0x172, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SysYParserParser::Initializer SysYParserParser::_init;
