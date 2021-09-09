// Generated from /home/why/SysYCompiler/src/frontend/SysY.g4 by ANTLR 4.8
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class SysYParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, Int=2, Void=3, Const=4, Return=5, If=6, Else=7, While=8, Break=9, 
		Continue=10, Lparen=11, Rparen=12, Lbrkt=13, Rbrkt=14, Lbrace=15, Rbrace=16, 
		Comma=17, Semicolon=18, Minus=19, Addition=20, Exclamation=21, Multiplication=22, 
		Division=23, Modulo=24, LAND=25, LOR=26, EQ=27, NEQ=28, LT=29, LE=30, 
		GT=31, GE=32, IntLiteral=33, Identifier=34, WS=35, LINE_COMMENT=36, COMMENT=37;
	public static final int
		RULE_compUnit = 0, RULE_decl = 1, RULE_constDecl = 2, RULE_bType = 3, 
		RULE_constDef = 4, RULE_constInitVal = 5, RULE_varDecl = 6, RULE_varDef = 7, 
		RULE_initVal = 8, RULE_funcDef = 9, RULE_funcType = 10, RULE_funcFParams = 11, 
		RULE_funcFParam = 12, RULE_block = 13, RULE_blockItem = 14, RULE_stmt = 15, 
		RULE_exp = 16, RULE_cond = 17, RULE_lVal = 18, RULE_primaryExp = 19, RULE_number = 20, 
		RULE_unaryExp = 21, RULE_unaryOp = 22, RULE_funcRParams = 23, RULE_funcRParam = 24, 
		RULE_mulExp = 25, RULE_addExp = 26, RULE_relExp = 27, RULE_eqExp = 28, 
		RULE_lAndExp = 29, RULE_lOrExp = 30, RULE_constExp = 31;
	private static String[] makeRuleNames() {
		return new String[] {
			"compUnit", "decl", "constDecl", "bType", "constDef", "constInitVal", 
			"varDecl", "varDef", "initVal", "funcDef", "funcType", "funcFParams", 
			"funcFParam", "block", "blockItem", "stmt", "exp", "cond", "lVal", "primaryExp", 
			"number", "unaryExp", "unaryOp", "funcRParams", "funcRParam", "mulExp", 
			"addExp", "relExp", "eqExp", "lAndExp", "lOrExp", "constExp"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'='", "'int'", "'void'", "'const'", "'return'", "'if'", "'else'", 
			"'while'", "'break'", "'continue'", "'('", "')'", "'['", "']'", "'{'", 
			"'}'", "','", "';'", "'-'", "'+'", "'!'", "'*'", "'/'", "'%'", "'&&'", 
			"'||'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, "Int", "Void", "Const", "Return", "If", "Else", "While", 
			"Break", "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", 
			"Rbrace", "Comma", "Semicolon", "Minus", "Addition", "Exclamation", "Multiplication", 
			"Division", "Modulo", "LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", 
			"IntLiteral", "Identifier", "WS", "LINE_COMMENT", "COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "SysY.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public SysYParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class CompUnitContext extends ParserRuleContext {
		public TerminalNode EOF() { return getToken(SysYParser.EOF, 0); }
		public List<DeclContext> decl() {
			return getRuleContexts(DeclContext.class);
		}
		public DeclContext decl(int i) {
			return getRuleContext(DeclContext.class,i);
		}
		public List<FuncDefContext> funcDef() {
			return getRuleContexts(FuncDefContext.class);
		}
		public FuncDefContext funcDef(int i) {
			return getRuleContext(FuncDefContext.class,i);
		}
		public CompUnitContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_compUnit; }
	}

	public final CompUnitContext compUnit() throws RecognitionException {
		CompUnitContext _localctx = new CompUnitContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_compUnit);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(68);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Int) | (1L << Void) | (1L << Const))) != 0)) {
				{
				setState(66);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,0,_ctx) ) {
				case 1:
					{
					setState(64);
					decl();
					}
					break;
				case 2:
					{
					setState(65);
					funcDef();
					}
					break;
				}
				}
				setState(70);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(71);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeclContext extends ParserRuleContext {
		public ConstDeclContext constDecl() {
			return getRuleContext(ConstDeclContext.class,0);
		}
		public VarDeclContext varDecl() {
			return getRuleContext(VarDeclContext.class,0);
		}
		public DeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_decl; }
	}

	public final DeclContext decl() throws RecognitionException {
		DeclContext _localctx = new DeclContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_decl);
		try {
			setState(75);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case Const:
				enterOuterAlt(_localctx, 1);
				{
				setState(73);
				constDecl();
				}
				break;
			case Int:
				enterOuterAlt(_localctx, 2);
				{
				setState(74);
				varDecl();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstDeclContext extends ParserRuleContext {
		public TerminalNode Const() { return getToken(SysYParser.Const, 0); }
		public BTypeContext bType() {
			return getRuleContext(BTypeContext.class,0);
		}
		public List<ConstDefContext> constDef() {
			return getRuleContexts(ConstDefContext.class);
		}
		public ConstDefContext constDef(int i) {
			return getRuleContext(ConstDefContext.class,i);
		}
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public List<TerminalNode> Comma() { return getTokens(SysYParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(SysYParser.Comma, i);
		}
		public ConstDeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constDecl; }
	}

	public final ConstDeclContext constDecl() throws RecognitionException {
		ConstDeclContext _localctx = new ConstDeclContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_constDecl);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(77);
			match(Const);
			setState(78);
			bType();
			setState(79);
			constDef();
			setState(84);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Comma) {
				{
				{
				setState(80);
				match(Comma);
				setState(81);
				constDef();
				}
				}
				setState(86);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(87);
			match(Semicolon);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BTypeContext extends ParserRuleContext {
		public TerminalNode Int() { return getToken(SysYParser.Int, 0); }
		public BTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_bType; }
	}

	public final BTypeContext bType() throws RecognitionException {
		BTypeContext _localctx = new BTypeContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_bType);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(89);
			match(Int);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstDefContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public ConstInitValContext constInitVal() {
			return getRuleContext(ConstInitValContext.class,0);
		}
		public List<TerminalNode> Lbrkt() { return getTokens(SysYParser.Lbrkt); }
		public TerminalNode Lbrkt(int i) {
			return getToken(SysYParser.Lbrkt, i);
		}
		public List<ConstExpContext> constExp() {
			return getRuleContexts(ConstExpContext.class);
		}
		public ConstExpContext constExp(int i) {
			return getRuleContext(ConstExpContext.class,i);
		}
		public List<TerminalNode> Rbrkt() { return getTokens(SysYParser.Rbrkt); }
		public TerminalNode Rbrkt(int i) {
			return getToken(SysYParser.Rbrkt, i);
		}
		public ConstDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constDef; }
	}

	public final ConstDefContext constDef() throws RecognitionException {
		ConstDefContext _localctx = new ConstDefContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_constDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(91);
			match(Identifier);
			setState(98);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Lbrkt) {
				{
				{
				setState(92);
				match(Lbrkt);
				setState(93);
				constExp();
				setState(94);
				match(Rbrkt);
				}
				}
				setState(100);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(101);
			match(T__0);
			setState(102);
			constInitVal();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstInitValContext extends ParserRuleContext {
		public ConstInitValContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constInitVal; }
	 
		public ConstInitValContext() { }
		public void copyFrom(ConstInitValContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ListConstInitValContext extends ConstInitValContext {
		public TerminalNode Lbrace() { return getToken(SysYParser.Lbrace, 0); }
		public TerminalNode Rbrace() { return getToken(SysYParser.Rbrace, 0); }
		public List<ConstInitValContext> constInitVal() {
			return getRuleContexts(ConstInitValContext.class);
		}
		public ConstInitValContext constInitVal(int i) {
			return getRuleContext(ConstInitValContext.class,i);
		}
		public List<TerminalNode> Comma() { return getTokens(SysYParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(SysYParser.Comma, i);
		}
		public ListConstInitValContext(ConstInitValContext ctx) { copyFrom(ctx); }
	}
	public static class ScalarConstInitValContext extends ConstInitValContext {
		public ConstExpContext constExp() {
			return getRuleContext(ConstExpContext.class,0);
		}
		public ScalarConstInitValContext(ConstInitValContext ctx) { copyFrom(ctx); }
	}

	public final ConstInitValContext constInitVal() throws RecognitionException {
		ConstInitValContext _localctx = new ConstInitValContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_constInitVal);
		int _la;
		try {
			setState(117);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case Lparen:
			case Minus:
			case Addition:
			case Exclamation:
			case IntLiteral:
			case Identifier:
				_localctx = new ScalarConstInitValContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(104);
				constExp();
				}
				break;
			case Lbrace:
				_localctx = new ListConstInitValContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(105);
				match(Lbrace);
				setState(114);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Lparen) | (1L << Lbrace) | (1L << Minus) | (1L << Addition) | (1L << Exclamation) | (1L << IntLiteral) | (1L << Identifier))) != 0)) {
					{
					setState(106);
					constInitVal();
					setState(111);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==Comma) {
						{
						{
						setState(107);
						match(Comma);
						setState(108);
						constInitVal();
						}
						}
						setState(113);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(116);
				match(Rbrace);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VarDeclContext extends ParserRuleContext {
		public BTypeContext bType() {
			return getRuleContext(BTypeContext.class,0);
		}
		public List<VarDefContext> varDef() {
			return getRuleContexts(VarDefContext.class);
		}
		public VarDefContext varDef(int i) {
			return getRuleContext(VarDefContext.class,i);
		}
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public List<TerminalNode> Comma() { return getTokens(SysYParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(SysYParser.Comma, i);
		}
		public VarDeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varDecl; }
	}

	public final VarDeclContext varDecl() throws RecognitionException {
		VarDeclContext _localctx = new VarDeclContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_varDecl);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(119);
			bType();
			setState(120);
			varDef();
			setState(125);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Comma) {
				{
				{
				setState(121);
				match(Comma);
				setState(122);
				varDef();
				}
				}
				setState(127);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(128);
			match(Semicolon);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VarDefContext extends ParserRuleContext {
		public VarDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varDef; }
	 
		public VarDefContext() { }
		public void copyFrom(VarDefContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class UninitVarDefContext extends VarDefContext {
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public List<TerminalNode> Lbrkt() { return getTokens(SysYParser.Lbrkt); }
		public TerminalNode Lbrkt(int i) {
			return getToken(SysYParser.Lbrkt, i);
		}
		public List<ConstExpContext> constExp() {
			return getRuleContexts(ConstExpContext.class);
		}
		public ConstExpContext constExp(int i) {
			return getRuleContext(ConstExpContext.class,i);
		}
		public List<TerminalNode> Rbrkt() { return getTokens(SysYParser.Rbrkt); }
		public TerminalNode Rbrkt(int i) {
			return getToken(SysYParser.Rbrkt, i);
		}
		public UninitVarDefContext(VarDefContext ctx) { copyFrom(ctx); }
	}
	public static class InitVarDefContext extends VarDefContext {
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public InitValContext initVal() {
			return getRuleContext(InitValContext.class,0);
		}
		public List<TerminalNode> Lbrkt() { return getTokens(SysYParser.Lbrkt); }
		public TerminalNode Lbrkt(int i) {
			return getToken(SysYParser.Lbrkt, i);
		}
		public List<ConstExpContext> constExp() {
			return getRuleContexts(ConstExpContext.class);
		}
		public ConstExpContext constExp(int i) {
			return getRuleContext(ConstExpContext.class,i);
		}
		public List<TerminalNode> Rbrkt() { return getTokens(SysYParser.Rbrkt); }
		public TerminalNode Rbrkt(int i) {
			return getToken(SysYParser.Rbrkt, i);
		}
		public InitVarDefContext(VarDefContext ctx) { copyFrom(ctx); }
	}

	public final VarDefContext varDef() throws RecognitionException {
		VarDefContext _localctx = new VarDefContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_varDef);
		int _la;
		try {
			setState(152);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,11,_ctx) ) {
			case 1:
				_localctx = new UninitVarDefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(130);
				match(Identifier);
				setState(137);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==Lbrkt) {
					{
					{
					setState(131);
					match(Lbrkt);
					setState(132);
					constExp();
					setState(133);
					match(Rbrkt);
					}
					}
					setState(139);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case 2:
				_localctx = new InitVarDefContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(140);
				match(Identifier);
				setState(147);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==Lbrkt) {
					{
					{
					setState(141);
					match(Lbrkt);
					setState(142);
					constExp();
					setState(143);
					match(Rbrkt);
					}
					}
					setState(149);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(150);
				match(T__0);
				setState(151);
				initVal();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class InitValContext extends ParserRuleContext {
		public InitValContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_initVal; }
	 
		public InitValContext() { }
		public void copyFrom(InitValContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ScalarInitValContext extends InitValContext {
		public ExpContext exp() {
			return getRuleContext(ExpContext.class,0);
		}
		public ScalarInitValContext(InitValContext ctx) { copyFrom(ctx); }
	}
	public static class ListInitvalContext extends InitValContext {
		public TerminalNode Lbrace() { return getToken(SysYParser.Lbrace, 0); }
		public TerminalNode Rbrace() { return getToken(SysYParser.Rbrace, 0); }
		public List<InitValContext> initVal() {
			return getRuleContexts(InitValContext.class);
		}
		public InitValContext initVal(int i) {
			return getRuleContext(InitValContext.class,i);
		}
		public List<TerminalNode> Comma() { return getTokens(SysYParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(SysYParser.Comma, i);
		}
		public ListInitvalContext(InitValContext ctx) { copyFrom(ctx); }
	}

	public final InitValContext initVal() throws RecognitionException {
		InitValContext _localctx = new InitValContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_initVal);
		int _la;
		try {
			setState(167);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case Lparen:
			case Minus:
			case Addition:
			case Exclamation:
			case IntLiteral:
			case Identifier:
				_localctx = new ScalarInitValContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(154);
				exp();
				}
				break;
			case Lbrace:
				_localctx = new ListInitvalContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(155);
				match(Lbrace);
				setState(164);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Lparen) | (1L << Lbrace) | (1L << Minus) | (1L << Addition) | (1L << Exclamation) | (1L << IntLiteral) | (1L << Identifier))) != 0)) {
					{
					setState(156);
					initVal();
					setState(161);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==Comma) {
						{
						{
						setState(157);
						match(Comma);
						setState(158);
						initVal();
						}
						}
						setState(163);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(166);
				match(Rbrace);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FuncDefContext extends ParserRuleContext {
		public FuncTypeContext funcType() {
			return getRuleContext(FuncTypeContext.class,0);
		}
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public TerminalNode Lparen() { return getToken(SysYParser.Lparen, 0); }
		public TerminalNode Rparen() { return getToken(SysYParser.Rparen, 0); }
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public FuncFParamsContext funcFParams() {
			return getRuleContext(FuncFParamsContext.class,0);
		}
		public FuncDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcDef; }
	}

	public final FuncDefContext funcDef() throws RecognitionException {
		FuncDefContext _localctx = new FuncDefContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_funcDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(169);
			funcType();
			setState(170);
			match(Identifier);
			setState(171);
			match(Lparen);
			setState(173);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==Int) {
				{
				setState(172);
				funcFParams();
				}
			}

			setState(175);
			match(Rparen);
			setState(176);
			block();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FuncTypeContext extends ParserRuleContext {
		public TerminalNode Void() { return getToken(SysYParser.Void, 0); }
		public TerminalNode Int() { return getToken(SysYParser.Int, 0); }
		public FuncTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcType; }
	}

	public final FuncTypeContext funcType() throws RecognitionException {
		FuncTypeContext _localctx = new FuncTypeContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_funcType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(178);
			_la = _input.LA(1);
			if ( !(_la==Int || _la==Void) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FuncFParamsContext extends ParserRuleContext {
		public List<FuncFParamContext> funcFParam() {
			return getRuleContexts(FuncFParamContext.class);
		}
		public FuncFParamContext funcFParam(int i) {
			return getRuleContext(FuncFParamContext.class,i);
		}
		public List<TerminalNode> Comma() { return getTokens(SysYParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(SysYParser.Comma, i);
		}
		public FuncFParamsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcFParams; }
	}

	public final FuncFParamsContext funcFParams() throws RecognitionException {
		FuncFParamsContext _localctx = new FuncFParamsContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_funcFParams);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(180);
			funcFParam();
			setState(185);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Comma) {
				{
				{
				setState(181);
				match(Comma);
				setState(182);
				funcFParam();
				}
				}
				setState(187);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FuncFParamContext extends ParserRuleContext {
		public BTypeContext bType() {
			return getRuleContext(BTypeContext.class,0);
		}
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public List<TerminalNode> Lbrkt() { return getTokens(SysYParser.Lbrkt); }
		public TerminalNode Lbrkt(int i) {
			return getToken(SysYParser.Lbrkt, i);
		}
		public List<TerminalNode> Rbrkt() { return getTokens(SysYParser.Rbrkt); }
		public TerminalNode Rbrkt(int i) {
			return getToken(SysYParser.Rbrkt, i);
		}
		public List<ConstExpContext> constExp() {
			return getRuleContexts(ConstExpContext.class);
		}
		public ConstExpContext constExp(int i) {
			return getRuleContext(ConstExpContext.class,i);
		}
		public FuncFParamContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcFParam; }
	}

	public final FuncFParamContext funcFParam() throws RecognitionException {
		FuncFParamContext _localctx = new FuncFParamContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_funcFParam);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(188);
			bType();
			setState(189);
			match(Identifier);
			setState(201);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==Lbrkt) {
				{
				setState(190);
				match(Lbrkt);
				setState(191);
				match(Rbrkt);
				setState(198);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==Lbrkt) {
					{
					{
					setState(192);
					match(Lbrkt);
					setState(193);
					constExp();
					setState(194);
					match(Rbrkt);
					}
					}
					setState(200);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlockContext extends ParserRuleContext {
		public TerminalNode Lbrace() { return getToken(SysYParser.Lbrace, 0); }
		public TerminalNode Rbrace() { return getToken(SysYParser.Rbrace, 0); }
		public List<BlockItemContext> blockItem() {
			return getRuleContexts(BlockItemContext.class);
		}
		public BlockItemContext blockItem(int i) {
			return getRuleContext(BlockItemContext.class,i);
		}
		public BlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_block; }
	}

	public final BlockContext block() throws RecognitionException {
		BlockContext _localctx = new BlockContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_block);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(203);
			match(Lbrace);
			setState(207);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Int) | (1L << Const) | (1L << Return) | (1L << If) | (1L << While) | (1L << Break) | (1L << Continue) | (1L << Lparen) | (1L << Lbrace) | (1L << Semicolon) | (1L << Minus) | (1L << Addition) | (1L << Exclamation) | (1L << IntLiteral) | (1L << Identifier))) != 0)) {
				{
				{
				setState(204);
				blockItem();
				}
				}
				setState(209);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(210);
			match(Rbrace);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlockItemContext extends ParserRuleContext {
		public DeclContext decl() {
			return getRuleContext(DeclContext.class,0);
		}
		public StmtContext stmt() {
			return getRuleContext(StmtContext.class,0);
		}
		public BlockItemContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_blockItem; }
	}

	public final BlockItemContext blockItem() throws RecognitionException {
		BlockItemContext _localctx = new BlockItemContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_blockItem);
		try {
			setState(214);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case Int:
			case Const:
				enterOuterAlt(_localctx, 1);
				{
				setState(212);
				decl();
				}
				break;
			case Return:
			case If:
			case While:
			case Break:
			case Continue:
			case Lparen:
			case Lbrace:
			case Semicolon:
			case Minus:
			case Addition:
			case Exclamation:
			case IntLiteral:
			case Identifier:
				enterOuterAlt(_localctx, 2);
				{
				setState(213);
				stmt();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StmtContext extends ParserRuleContext {
		public StmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stmt; }
	 
		public StmtContext() { }
		public void copyFrom(StmtContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class WhileStmtContext extends StmtContext {
		public TerminalNode While() { return getToken(SysYParser.While, 0); }
		public TerminalNode Lparen() { return getToken(SysYParser.Lparen, 0); }
		public CondContext cond() {
			return getRuleContext(CondContext.class,0);
		}
		public TerminalNode Rparen() { return getToken(SysYParser.Rparen, 0); }
		public StmtContext stmt() {
			return getRuleContext(StmtContext.class,0);
		}
		public WhileStmtContext(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class BlockStmtContext extends StmtContext {
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public BlockStmtContext(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class AssignmentContext extends StmtContext {
		public LValContext lVal() {
			return getRuleContext(LValContext.class,0);
		}
		public ExpContext exp() {
			return getRuleContext(ExpContext.class,0);
		}
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public AssignmentContext(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class IfStmt1Context extends StmtContext {
		public TerminalNode If() { return getToken(SysYParser.If, 0); }
		public TerminalNode Lparen() { return getToken(SysYParser.Lparen, 0); }
		public CondContext cond() {
			return getRuleContext(CondContext.class,0);
		}
		public TerminalNode Rparen() { return getToken(SysYParser.Rparen, 0); }
		public StmtContext stmt() {
			return getRuleContext(StmtContext.class,0);
		}
		public IfStmt1Context(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class BreakStmtContext extends StmtContext {
		public TerminalNode Break() { return getToken(SysYParser.Break, 0); }
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public BreakStmtContext(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class ExpStmtContext extends StmtContext {
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public ExpContext exp() {
			return getRuleContext(ExpContext.class,0);
		}
		public ExpStmtContext(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class IfStmt2Context extends StmtContext {
		public TerminalNode If() { return getToken(SysYParser.If, 0); }
		public TerminalNode Lparen() { return getToken(SysYParser.Lparen, 0); }
		public CondContext cond() {
			return getRuleContext(CondContext.class,0);
		}
		public TerminalNode Rparen() { return getToken(SysYParser.Rparen, 0); }
		public List<StmtContext> stmt() {
			return getRuleContexts(StmtContext.class);
		}
		public StmtContext stmt(int i) {
			return getRuleContext(StmtContext.class,i);
		}
		public TerminalNode Else() { return getToken(SysYParser.Else, 0); }
		public IfStmt2Context(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class ReturnStmtContext extends StmtContext {
		public TerminalNode Return() { return getToken(SysYParser.Return, 0); }
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public ExpContext exp() {
			return getRuleContext(ExpContext.class,0);
		}
		public ReturnStmtContext(StmtContext ctx) { copyFrom(ctx); }
	}
	public static class ContinueStmtContext extends StmtContext {
		public TerminalNode Continue() { return getToken(SysYParser.Continue, 0); }
		public TerminalNode Semicolon() { return getToken(SysYParser.Semicolon, 0); }
		public ContinueStmtContext(StmtContext ctx) { copyFrom(ctx); }
	}

	public final StmtContext stmt() throws RecognitionException {
		StmtContext _localctx = new StmtContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_stmt);
		int _la;
		try {
			setState(255);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,23,_ctx) ) {
			case 1:
				_localctx = new AssignmentContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(216);
				lVal();
				setState(217);
				match(T__0);
				setState(218);
				exp();
				setState(219);
				match(Semicolon);
				}
				break;
			case 2:
				_localctx = new ExpStmtContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(222);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Lparen) | (1L << Minus) | (1L << Addition) | (1L << Exclamation) | (1L << IntLiteral) | (1L << Identifier))) != 0)) {
					{
					setState(221);
					exp();
					}
				}

				setState(224);
				match(Semicolon);
				}
				break;
			case 3:
				_localctx = new BlockStmtContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(225);
				block();
				}
				break;
			case 4:
				_localctx = new IfStmt1Context(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(226);
				match(If);
				setState(227);
				match(Lparen);
				setState(228);
				cond();
				setState(229);
				match(Rparen);
				setState(230);
				stmt();
				}
				break;
			case 5:
				_localctx = new IfStmt2Context(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(232);
				match(If);
				setState(233);
				match(Lparen);
				setState(234);
				cond();
				setState(235);
				match(Rparen);
				setState(236);
				stmt();
				setState(237);
				match(Else);
				setState(238);
				stmt();
				}
				break;
			case 6:
				_localctx = new WhileStmtContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(240);
				match(While);
				setState(241);
				match(Lparen);
				setState(242);
				cond();
				setState(243);
				match(Rparen);
				setState(244);
				stmt();
				}
				break;
			case 7:
				_localctx = new BreakStmtContext(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(246);
				match(Break);
				setState(247);
				match(Semicolon);
				}
				break;
			case 8:
				_localctx = new ContinueStmtContext(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(248);
				match(Continue);
				setState(249);
				match(Semicolon);
				}
				break;
			case 9:
				_localctx = new ReturnStmtContext(_localctx);
				enterOuterAlt(_localctx, 9);
				{
				setState(250);
				match(Return);
				setState(252);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Lparen) | (1L << Minus) | (1L << Addition) | (1L << Exclamation) | (1L << IntLiteral) | (1L << Identifier))) != 0)) {
					{
					setState(251);
					exp();
					}
				}

				setState(254);
				match(Semicolon);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExpContext extends ParserRuleContext {
		public AddExpContext addExp() {
			return getRuleContext(AddExpContext.class,0);
		}
		public ExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_exp; }
	}

	public final ExpContext exp() throws RecognitionException {
		ExpContext _localctx = new ExpContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_exp);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(257);
			addExp(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class CondContext extends ParserRuleContext {
		public LOrExpContext lOrExp() {
			return getRuleContext(LOrExpContext.class,0);
		}
		public CondContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_cond; }
	}

	public final CondContext cond() throws RecognitionException {
		CondContext _localctx = new CondContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_cond);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(259);
			lOrExp(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LValContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public List<TerminalNode> Lbrkt() { return getTokens(SysYParser.Lbrkt); }
		public TerminalNode Lbrkt(int i) {
			return getToken(SysYParser.Lbrkt, i);
		}
		public List<ExpContext> exp() {
			return getRuleContexts(ExpContext.class);
		}
		public ExpContext exp(int i) {
			return getRuleContext(ExpContext.class,i);
		}
		public List<TerminalNode> Rbrkt() { return getTokens(SysYParser.Rbrkt); }
		public TerminalNode Rbrkt(int i) {
			return getToken(SysYParser.Rbrkt, i);
		}
		public LValContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_lVal; }
	}

	public final LValContext lVal() throws RecognitionException {
		LValContext _localctx = new LValContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_lVal);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(261);
			match(Identifier);
			setState(268);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,24,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(262);
					match(Lbrkt);
					setState(263);
					exp();
					setState(264);
					match(Rbrkt);
					}
					} 
				}
				setState(270);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,24,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrimaryExpContext extends ParserRuleContext {
		public PrimaryExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_primaryExp; }
	 
		public PrimaryExpContext() { }
		public void copyFrom(PrimaryExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class PrimaryExp2Context extends PrimaryExpContext {
		public LValContext lVal() {
			return getRuleContext(LValContext.class,0);
		}
		public PrimaryExp2Context(PrimaryExpContext ctx) { copyFrom(ctx); }
	}
	public static class PrimaryExp1Context extends PrimaryExpContext {
		public TerminalNode Lparen() { return getToken(SysYParser.Lparen, 0); }
		public ExpContext exp() {
			return getRuleContext(ExpContext.class,0);
		}
		public TerminalNode Rparen() { return getToken(SysYParser.Rparen, 0); }
		public PrimaryExp1Context(PrimaryExpContext ctx) { copyFrom(ctx); }
	}
	public static class PrimaryExp3Context extends PrimaryExpContext {
		public NumberContext number() {
			return getRuleContext(NumberContext.class,0);
		}
		public PrimaryExp3Context(PrimaryExpContext ctx) { copyFrom(ctx); }
	}

	public final PrimaryExpContext primaryExp() throws RecognitionException {
		PrimaryExpContext _localctx = new PrimaryExpContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_primaryExp);
		try {
			setState(277);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case Lparen:
				_localctx = new PrimaryExp1Context(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(271);
				match(Lparen);
				setState(272);
				exp();
				setState(273);
				match(Rparen);
				}
				break;
			case Identifier:
				_localctx = new PrimaryExp2Context(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(275);
				lVal();
				}
				break;
			case IntLiteral:
				_localctx = new PrimaryExp3Context(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(276);
				number();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumberContext extends ParserRuleContext {
		public TerminalNode IntLiteral() { return getToken(SysYParser.IntLiteral, 0); }
		public NumberContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_number; }
	}

	public final NumberContext number() throws RecognitionException {
		NumberContext _localctx = new NumberContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_number);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(279);
			match(IntLiteral);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class UnaryExpContext extends ParserRuleContext {
		public UnaryExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unaryExp; }
	 
		public UnaryExpContext() { }
		public void copyFrom(UnaryExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class Unary1Context extends UnaryExpContext {
		public PrimaryExpContext primaryExp() {
			return getRuleContext(PrimaryExpContext.class,0);
		}
		public Unary1Context(UnaryExpContext ctx) { copyFrom(ctx); }
	}
	public static class Unary2Context extends UnaryExpContext {
		public TerminalNode Identifier() { return getToken(SysYParser.Identifier, 0); }
		public TerminalNode Lparen() { return getToken(SysYParser.Lparen, 0); }
		public TerminalNode Rparen() { return getToken(SysYParser.Rparen, 0); }
		public FuncRParamsContext funcRParams() {
			return getRuleContext(FuncRParamsContext.class,0);
		}
		public Unary2Context(UnaryExpContext ctx) { copyFrom(ctx); }
	}
	public static class Unary3Context extends UnaryExpContext {
		public UnaryOpContext unaryOp() {
			return getRuleContext(UnaryOpContext.class,0);
		}
		public UnaryExpContext unaryExp() {
			return getRuleContext(UnaryExpContext.class,0);
		}
		public Unary3Context(UnaryExpContext ctx) { copyFrom(ctx); }
	}

	public final UnaryExpContext unaryExp() throws RecognitionException {
		UnaryExpContext _localctx = new UnaryExpContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_unaryExp);
		int _la;
		try {
			setState(291);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,27,_ctx) ) {
			case 1:
				_localctx = new Unary1Context(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(281);
				primaryExp();
				}
				break;
			case 2:
				_localctx = new Unary2Context(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(282);
				match(Identifier);
				setState(283);
				match(Lparen);
				setState(285);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Lparen) | (1L << Minus) | (1L << Addition) | (1L << Exclamation) | (1L << IntLiteral) | (1L << Identifier))) != 0)) {
					{
					setState(284);
					funcRParams();
					}
				}

				setState(287);
				match(Rparen);
				}
				break;
			case 3:
				_localctx = new Unary3Context(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(288);
				unaryOp();
				setState(289);
				unaryExp();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class UnaryOpContext extends ParserRuleContext {
		public TerminalNode Addition() { return getToken(SysYParser.Addition, 0); }
		public TerminalNode Minus() { return getToken(SysYParser.Minus, 0); }
		public TerminalNode Exclamation() { return getToken(SysYParser.Exclamation, 0); }
		public UnaryOpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unaryOp; }
	}

	public final UnaryOpContext unaryOp() throws RecognitionException {
		UnaryOpContext _localctx = new UnaryOpContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_unaryOp);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(293);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Minus) | (1L << Addition) | (1L << Exclamation))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FuncRParamsContext extends ParserRuleContext {
		public List<FuncRParamContext> funcRParam() {
			return getRuleContexts(FuncRParamContext.class);
		}
		public FuncRParamContext funcRParam(int i) {
			return getRuleContext(FuncRParamContext.class,i);
		}
		public List<TerminalNode> Comma() { return getTokens(SysYParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(SysYParser.Comma, i);
		}
		public FuncRParamsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcRParams; }
	}

	public final FuncRParamsContext funcRParams() throws RecognitionException {
		FuncRParamsContext _localctx = new FuncRParamsContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_funcRParams);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(295);
			funcRParam();
			setState(300);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Comma) {
				{
				{
				setState(296);
				match(Comma);
				setState(297);
				funcRParam();
				}
				}
				setState(302);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FuncRParamContext extends ParserRuleContext {
		public ExpContext exp() {
			return getRuleContext(ExpContext.class,0);
		}
		public FuncRParamContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcRParam; }
	}

	public final FuncRParamContext funcRParam() throws RecognitionException {
		FuncRParamContext _localctx = new FuncRParamContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_funcRParam);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(303);
			exp();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MulExpContext extends ParserRuleContext {
		public MulExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mulExp; }
	 
		public MulExpContext() { }
		public void copyFrom(MulExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class Mul2Context extends MulExpContext {
		public MulExpContext mulExp() {
			return getRuleContext(MulExpContext.class,0);
		}
		public UnaryExpContext unaryExp() {
			return getRuleContext(UnaryExpContext.class,0);
		}
		public TerminalNode Multiplication() { return getToken(SysYParser.Multiplication, 0); }
		public TerminalNode Division() { return getToken(SysYParser.Division, 0); }
		public TerminalNode Modulo() { return getToken(SysYParser.Modulo, 0); }
		public Mul2Context(MulExpContext ctx) { copyFrom(ctx); }
	}
	public static class Mul1Context extends MulExpContext {
		public UnaryExpContext unaryExp() {
			return getRuleContext(UnaryExpContext.class,0);
		}
		public Mul1Context(MulExpContext ctx) { copyFrom(ctx); }
	}

	public final MulExpContext mulExp() throws RecognitionException {
		return mulExp(0);
	}

	private MulExpContext mulExp(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		MulExpContext _localctx = new MulExpContext(_ctx, _parentState);
		MulExpContext _prevctx = _localctx;
		int _startState = 50;
		enterRecursionRule(_localctx, 50, RULE_mulExp, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			_localctx = new Mul1Context(_localctx);
			_ctx = _localctx;
			_prevctx = _localctx;

			setState(306);
			unaryExp();
			}
			_ctx.stop = _input.LT(-1);
			setState(313);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,29,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new Mul2Context(new MulExpContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_mulExp);
					setState(308);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(309);
					_la = _input.LA(1);
					if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Multiplication) | (1L << Division) | (1L << Modulo))) != 0)) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					setState(310);
					unaryExp();
					}
					} 
				}
				setState(315);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,29,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class AddExpContext extends ParserRuleContext {
		public AddExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_addExp; }
	 
		public AddExpContext() { }
		public void copyFrom(AddExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class Add2Context extends AddExpContext {
		public AddExpContext addExp() {
			return getRuleContext(AddExpContext.class,0);
		}
		public MulExpContext mulExp() {
			return getRuleContext(MulExpContext.class,0);
		}
		public TerminalNode Addition() { return getToken(SysYParser.Addition, 0); }
		public TerminalNode Minus() { return getToken(SysYParser.Minus, 0); }
		public Add2Context(AddExpContext ctx) { copyFrom(ctx); }
	}
	public static class Add1Context extends AddExpContext {
		public MulExpContext mulExp() {
			return getRuleContext(MulExpContext.class,0);
		}
		public Add1Context(AddExpContext ctx) { copyFrom(ctx); }
	}

	public final AddExpContext addExp() throws RecognitionException {
		return addExp(0);
	}

	private AddExpContext addExp(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		AddExpContext _localctx = new AddExpContext(_ctx, _parentState);
		AddExpContext _prevctx = _localctx;
		int _startState = 52;
		enterRecursionRule(_localctx, 52, RULE_addExp, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			_localctx = new Add1Context(_localctx);
			_ctx = _localctx;
			_prevctx = _localctx;

			setState(317);
			mulExp(0);
			}
			_ctx.stop = _input.LT(-1);
			setState(324);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,30,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new Add2Context(new AddExpContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_addExp);
					setState(319);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(320);
					_la = _input.LA(1);
					if ( !(_la==Minus || _la==Addition) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					setState(321);
					mulExp(0);
					}
					} 
				}
				setState(326);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,30,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class RelExpContext extends ParserRuleContext {
		public RelExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_relExp; }
	 
		public RelExpContext() { }
		public void copyFrom(RelExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class Rel2Context extends RelExpContext {
		public RelExpContext relExp() {
			return getRuleContext(RelExpContext.class,0);
		}
		public AddExpContext addExp() {
			return getRuleContext(AddExpContext.class,0);
		}
		public TerminalNode LT() { return getToken(SysYParser.LT, 0); }
		public TerminalNode GT() { return getToken(SysYParser.GT, 0); }
		public TerminalNode LE() { return getToken(SysYParser.LE, 0); }
		public TerminalNode GE() { return getToken(SysYParser.GE, 0); }
		public Rel2Context(RelExpContext ctx) { copyFrom(ctx); }
	}
	public static class Rel1Context extends RelExpContext {
		public AddExpContext addExp() {
			return getRuleContext(AddExpContext.class,0);
		}
		public Rel1Context(RelExpContext ctx) { copyFrom(ctx); }
	}

	public final RelExpContext relExp() throws RecognitionException {
		return relExp(0);
	}

	private RelExpContext relExp(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		RelExpContext _localctx = new RelExpContext(_ctx, _parentState);
		RelExpContext _prevctx = _localctx;
		int _startState = 54;
		enterRecursionRule(_localctx, 54, RULE_relExp, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			_localctx = new Rel1Context(_localctx);
			_ctx = _localctx;
			_prevctx = _localctx;

			setState(328);
			addExp(0);
			}
			_ctx.stop = _input.LT(-1);
			setState(335);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,31,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new Rel2Context(new RelExpContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_relExp);
					setState(330);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(331);
					_la = _input.LA(1);
					if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << LT) | (1L << LE) | (1L << GT) | (1L << GE))) != 0)) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					setState(332);
					addExp(0);
					}
					} 
				}
				setState(337);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,31,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class EqExpContext extends ParserRuleContext {
		public EqExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_eqExp; }
	 
		public EqExpContext() { }
		public void copyFrom(EqExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class Eq1Context extends EqExpContext {
		public RelExpContext relExp() {
			return getRuleContext(RelExpContext.class,0);
		}
		public Eq1Context(EqExpContext ctx) { copyFrom(ctx); }
	}
	public static class Eq2Context extends EqExpContext {
		public EqExpContext eqExp() {
			return getRuleContext(EqExpContext.class,0);
		}
		public RelExpContext relExp() {
			return getRuleContext(RelExpContext.class,0);
		}
		public TerminalNode EQ() { return getToken(SysYParser.EQ, 0); }
		public TerminalNode NEQ() { return getToken(SysYParser.NEQ, 0); }
		public Eq2Context(EqExpContext ctx) { copyFrom(ctx); }
	}

	public final EqExpContext eqExp() throws RecognitionException {
		return eqExp(0);
	}

	private EqExpContext eqExp(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		EqExpContext _localctx = new EqExpContext(_ctx, _parentState);
		EqExpContext _prevctx = _localctx;
		int _startState = 56;
		enterRecursionRule(_localctx, 56, RULE_eqExp, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			_localctx = new Eq1Context(_localctx);
			_ctx = _localctx;
			_prevctx = _localctx;

			setState(339);
			relExp(0);
			}
			_ctx.stop = _input.LT(-1);
			setState(346);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,32,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new Eq2Context(new EqExpContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_eqExp);
					setState(341);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(342);
					_la = _input.LA(1);
					if ( !(_la==EQ || _la==NEQ) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					setState(343);
					relExp(0);
					}
					} 
				}
				setState(348);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,32,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class LAndExpContext extends ParserRuleContext {
		public LAndExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_lAndExp; }
	 
		public LAndExpContext() { }
		public void copyFrom(LAndExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class LAnd2Context extends LAndExpContext {
		public LAndExpContext lAndExp() {
			return getRuleContext(LAndExpContext.class,0);
		}
		public TerminalNode LAND() { return getToken(SysYParser.LAND, 0); }
		public EqExpContext eqExp() {
			return getRuleContext(EqExpContext.class,0);
		}
		public LAnd2Context(LAndExpContext ctx) { copyFrom(ctx); }
	}
	public static class LAnd1Context extends LAndExpContext {
		public EqExpContext eqExp() {
			return getRuleContext(EqExpContext.class,0);
		}
		public LAnd1Context(LAndExpContext ctx) { copyFrom(ctx); }
	}

	public final LAndExpContext lAndExp() throws RecognitionException {
		return lAndExp(0);
	}

	private LAndExpContext lAndExp(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		LAndExpContext _localctx = new LAndExpContext(_ctx, _parentState);
		LAndExpContext _prevctx = _localctx;
		int _startState = 58;
		enterRecursionRule(_localctx, 58, RULE_lAndExp, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			_localctx = new LAnd1Context(_localctx);
			_ctx = _localctx;
			_prevctx = _localctx;

			setState(350);
			eqExp(0);
			}
			_ctx.stop = _input.LT(-1);
			setState(357);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,33,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new LAnd2Context(new LAndExpContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_lAndExp);
					setState(352);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(353);
					match(LAND);
					setState(354);
					eqExp(0);
					}
					} 
				}
				setState(359);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,33,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class LOrExpContext extends ParserRuleContext {
		public LOrExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_lOrExp; }
	 
		public LOrExpContext() { }
		public void copyFrom(LOrExpContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class LOr1Context extends LOrExpContext {
		public LAndExpContext lAndExp() {
			return getRuleContext(LAndExpContext.class,0);
		}
		public LOr1Context(LOrExpContext ctx) { copyFrom(ctx); }
	}
	public static class LOr2Context extends LOrExpContext {
		public LOrExpContext lOrExp() {
			return getRuleContext(LOrExpContext.class,0);
		}
		public TerminalNode LOR() { return getToken(SysYParser.LOR, 0); }
		public LAndExpContext lAndExp() {
			return getRuleContext(LAndExpContext.class,0);
		}
		public LOr2Context(LOrExpContext ctx) { copyFrom(ctx); }
	}

	public final LOrExpContext lOrExp() throws RecognitionException {
		return lOrExp(0);
	}

	private LOrExpContext lOrExp(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		LOrExpContext _localctx = new LOrExpContext(_ctx, _parentState);
		LOrExpContext _prevctx = _localctx;
		int _startState = 60;
		enterRecursionRule(_localctx, 60, RULE_lOrExp, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			_localctx = new LOr1Context(_localctx);
			_ctx = _localctx;
			_prevctx = _localctx;

			setState(361);
			lAndExp(0);
			}
			_ctx.stop = _input.LT(-1);
			setState(368);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,34,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new LOr2Context(new LOrExpContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_lOrExp);
					setState(363);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(364);
					match(LOR);
					setState(365);
					lAndExp(0);
					}
					} 
				}
				setState(370);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,34,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class ConstExpContext extends ParserRuleContext {
		public AddExpContext addExp() {
			return getRuleContext(AddExpContext.class,0);
		}
		public ConstExpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constExp; }
	}

	public final ConstExpContext constExp() throws RecognitionException {
		ConstExpContext _localctx = new ConstExpContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_constExp);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(371);
			addExp(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 25:
			return mulExp_sempred((MulExpContext)_localctx, predIndex);
		case 26:
			return addExp_sempred((AddExpContext)_localctx, predIndex);
		case 27:
			return relExp_sempred((RelExpContext)_localctx, predIndex);
		case 28:
			return eqExp_sempred((EqExpContext)_localctx, predIndex);
		case 29:
			return lAndExp_sempred((LAndExpContext)_localctx, predIndex);
		case 30:
			return lOrExp_sempred((LOrExpContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean mulExp_sempred(MulExpContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean addExp_sempred(AddExpContext _localctx, int predIndex) {
		switch (predIndex) {
		case 1:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean relExp_sempred(RelExpContext _localctx, int predIndex) {
		switch (predIndex) {
		case 2:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean eqExp_sempred(EqExpContext _localctx, int predIndex) {
		switch (predIndex) {
		case 3:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean lAndExp_sempred(LAndExpContext _localctx, int predIndex) {
		switch (predIndex) {
		case 4:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean lOrExp_sempred(LOrExpContext _localctx, int predIndex) {
		switch (predIndex) {
		case 5:
			return precpred(_ctx, 1);
		}
		return true;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\'\u0178\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\3\2\3\2\7\2E\n\2\f\2\16\2H\13\2\3\2\3\2\3\3\3\3\5\3N\n\3\3\4\3\4\3"+
		"\4\3\4\3\4\7\4U\n\4\f\4\16\4X\13\4\3\4\3\4\3\5\3\5\3\6\3\6\3\6\3\6\3\6"+
		"\7\6c\n\6\f\6\16\6f\13\6\3\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\7\7p\n\7\f\7"+
		"\16\7s\13\7\5\7u\n\7\3\7\5\7x\n\7\3\b\3\b\3\b\3\b\7\b~\n\b\f\b\16\b\u0081"+
		"\13\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\7\t\u008a\n\t\f\t\16\t\u008d\13\t\3"+
		"\t\3\t\3\t\3\t\3\t\7\t\u0094\n\t\f\t\16\t\u0097\13\t\3\t\3\t\5\t\u009b"+
		"\n\t\3\n\3\n\3\n\3\n\3\n\7\n\u00a2\n\n\f\n\16\n\u00a5\13\n\5\n\u00a7\n"+
		"\n\3\n\5\n\u00aa\n\n\3\13\3\13\3\13\3\13\5\13\u00b0\n\13\3\13\3\13\3\13"+
		"\3\f\3\f\3\r\3\r\3\r\7\r\u00ba\n\r\f\r\16\r\u00bd\13\r\3\16\3\16\3\16"+
		"\3\16\3\16\3\16\3\16\3\16\7\16\u00c7\n\16\f\16\16\16\u00ca\13\16\5\16"+
		"\u00cc\n\16\3\17\3\17\7\17\u00d0\n\17\f\17\16\17\u00d3\13\17\3\17\3\17"+
		"\3\20\3\20\5\20\u00d9\n\20\3\21\3\21\3\21\3\21\3\21\3\21\5\21\u00e1\n"+
		"\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3"+
		"\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3"+
		"\21\5\21\u00ff\n\21\3\21\5\21\u0102\n\21\3\22\3\22\3\23\3\23\3\24\3\24"+
		"\3\24\3\24\3\24\7\24\u010d\n\24\f\24\16\24\u0110\13\24\3\25\3\25\3\25"+
		"\3\25\3\25\3\25\5\25\u0118\n\25\3\26\3\26\3\27\3\27\3\27\3\27\5\27\u0120"+
		"\n\27\3\27\3\27\3\27\3\27\5\27\u0126\n\27\3\30\3\30\3\31\3\31\3\31\7\31"+
		"\u012d\n\31\f\31\16\31\u0130\13\31\3\32\3\32\3\33\3\33\3\33\3\33\3\33"+
		"\3\33\7\33\u013a\n\33\f\33\16\33\u013d\13\33\3\34\3\34\3\34\3\34\3\34"+
		"\3\34\7\34\u0145\n\34\f\34\16\34\u0148\13\34\3\35\3\35\3\35\3\35\3\35"+
		"\3\35\7\35\u0150\n\35\f\35\16\35\u0153\13\35\3\36\3\36\3\36\3\36\3\36"+
		"\3\36\7\36\u015b\n\36\f\36\16\36\u015e\13\36\3\37\3\37\3\37\3\37\3\37"+
		"\3\37\7\37\u0166\n\37\f\37\16\37\u0169\13\37\3 \3 \3 \3 \3 \3 \7 \u0171"+
		"\n \f \16 \u0174\13 \3!\3!\3!\2\b\64\668:<>\"\2\4\6\b\n\f\16\20\22\24"+
		"\26\30\32\34\36 \"$&(*,.\60\62\64\668:<>@\2\b\3\2\4\5\3\2\25\27\3\2\30"+
		"\32\3\2\25\26\3\2\37\"\3\2\35\36\2\u0183\2F\3\2\2\2\4M\3\2\2\2\6O\3\2"+
		"\2\2\b[\3\2\2\2\n]\3\2\2\2\fw\3\2\2\2\16y\3\2\2\2\20\u009a\3\2\2\2\22"+
		"\u00a9\3\2\2\2\24\u00ab\3\2\2\2\26\u00b4\3\2\2\2\30\u00b6\3\2\2\2\32\u00be"+
		"\3\2\2\2\34\u00cd\3\2\2\2\36\u00d8\3\2\2\2 \u0101\3\2\2\2\"\u0103\3\2"+
		"\2\2$\u0105\3\2\2\2&\u0107\3\2\2\2(\u0117\3\2\2\2*\u0119\3\2\2\2,\u0125"+
		"\3\2\2\2.\u0127\3\2\2\2\60\u0129\3\2\2\2\62\u0131\3\2\2\2\64\u0133\3\2"+
		"\2\2\66\u013e\3\2\2\28\u0149\3\2\2\2:\u0154\3\2\2\2<\u015f\3\2\2\2>\u016a"+
		"\3\2\2\2@\u0175\3\2\2\2BE\5\4\3\2CE\5\24\13\2DB\3\2\2\2DC\3\2\2\2EH\3"+
		"\2\2\2FD\3\2\2\2FG\3\2\2\2GI\3\2\2\2HF\3\2\2\2IJ\7\2\2\3J\3\3\2\2\2KN"+
		"\5\6\4\2LN\5\16\b\2MK\3\2\2\2ML\3\2\2\2N\5\3\2\2\2OP\7\6\2\2PQ\5\b\5\2"+
		"QV\5\n\6\2RS\7\23\2\2SU\5\n\6\2TR\3\2\2\2UX\3\2\2\2VT\3\2\2\2VW\3\2\2"+
		"\2WY\3\2\2\2XV\3\2\2\2YZ\7\24\2\2Z\7\3\2\2\2[\\\7\4\2\2\\\t\3\2\2\2]d"+
		"\7$\2\2^_\7\17\2\2_`\5@!\2`a\7\20\2\2ac\3\2\2\2b^\3\2\2\2cf\3\2\2\2db"+
		"\3\2\2\2de\3\2\2\2eg\3\2\2\2fd\3\2\2\2gh\7\3\2\2hi\5\f\7\2i\13\3\2\2\2"+
		"jx\5@!\2kt\7\21\2\2lq\5\f\7\2mn\7\23\2\2np\5\f\7\2om\3\2\2\2ps\3\2\2\2"+
		"qo\3\2\2\2qr\3\2\2\2ru\3\2\2\2sq\3\2\2\2tl\3\2\2\2tu\3\2\2\2uv\3\2\2\2"+
		"vx\7\22\2\2wj\3\2\2\2wk\3\2\2\2x\r\3\2\2\2yz\5\b\5\2z\177\5\20\t\2{|\7"+
		"\23\2\2|~\5\20\t\2}{\3\2\2\2~\u0081\3\2\2\2\177}\3\2\2\2\177\u0080\3\2"+
		"\2\2\u0080\u0082\3\2\2\2\u0081\177\3\2\2\2\u0082\u0083\7\24\2\2\u0083"+
		"\17\3\2\2\2\u0084\u008b\7$\2\2\u0085\u0086\7\17\2\2\u0086\u0087\5@!\2"+
		"\u0087\u0088\7\20\2\2\u0088\u008a\3\2\2\2\u0089\u0085\3\2\2\2\u008a\u008d"+
		"\3\2\2\2\u008b\u0089\3\2\2\2\u008b\u008c\3\2\2\2\u008c\u009b\3\2\2\2\u008d"+
		"\u008b\3\2\2\2\u008e\u0095\7$\2\2\u008f\u0090\7\17\2\2\u0090\u0091\5@"+
		"!\2\u0091\u0092\7\20\2\2\u0092\u0094\3\2\2\2\u0093\u008f\3\2\2\2\u0094"+
		"\u0097\3\2\2\2\u0095\u0093\3\2\2\2\u0095\u0096\3\2\2\2\u0096\u0098\3\2"+
		"\2\2\u0097\u0095\3\2\2\2\u0098\u0099\7\3\2\2\u0099\u009b\5\22\n\2\u009a"+
		"\u0084\3\2\2\2\u009a\u008e\3\2\2\2\u009b\21\3\2\2\2\u009c\u00aa\5\"\22"+
		"\2\u009d\u00a6\7\21\2\2\u009e\u00a3\5\22\n\2\u009f\u00a0\7\23\2\2\u00a0"+
		"\u00a2\5\22\n\2\u00a1\u009f\3\2\2\2\u00a2\u00a5\3\2\2\2\u00a3\u00a1\3"+
		"\2\2\2\u00a3\u00a4\3\2\2\2\u00a4\u00a7\3\2\2\2\u00a5\u00a3\3\2\2\2\u00a6"+
		"\u009e\3\2\2\2\u00a6\u00a7\3\2\2\2\u00a7\u00a8\3\2\2\2\u00a8\u00aa\7\22"+
		"\2\2\u00a9\u009c\3\2\2\2\u00a9\u009d\3\2\2\2\u00aa\23\3\2\2\2\u00ab\u00ac"+
		"\5\26\f\2\u00ac\u00ad\7$\2\2\u00ad\u00af\7\r\2\2\u00ae\u00b0\5\30\r\2"+
		"\u00af\u00ae\3\2\2\2\u00af\u00b0\3\2\2\2\u00b0\u00b1\3\2\2\2\u00b1\u00b2"+
		"\7\16\2\2\u00b2\u00b3\5\34\17\2\u00b3\25\3\2\2\2\u00b4\u00b5\t\2\2\2\u00b5"+
		"\27\3\2\2\2\u00b6\u00bb\5\32\16\2\u00b7\u00b8\7\23\2\2\u00b8\u00ba\5\32"+
		"\16\2\u00b9\u00b7\3\2\2\2\u00ba\u00bd\3\2\2\2\u00bb\u00b9\3\2\2\2\u00bb"+
		"\u00bc\3\2\2\2\u00bc\31\3\2\2\2\u00bd\u00bb\3\2\2\2\u00be\u00bf\5\b\5"+
		"\2\u00bf\u00cb\7$\2\2\u00c0\u00c1\7\17\2\2\u00c1\u00c8\7\20\2\2\u00c2"+
		"\u00c3\7\17\2\2\u00c3\u00c4\5@!\2\u00c4\u00c5\7\20\2\2\u00c5\u00c7\3\2"+
		"\2\2\u00c6\u00c2\3\2\2\2\u00c7\u00ca\3\2\2\2\u00c8\u00c6\3\2\2\2\u00c8"+
		"\u00c9\3\2\2\2\u00c9\u00cc\3\2\2\2\u00ca\u00c8\3\2\2\2\u00cb\u00c0\3\2"+
		"\2\2\u00cb\u00cc\3\2\2\2\u00cc\33\3\2\2\2\u00cd\u00d1\7\21\2\2\u00ce\u00d0"+
		"\5\36\20\2\u00cf\u00ce\3\2\2\2\u00d0\u00d3\3\2\2\2\u00d1\u00cf\3\2\2\2"+
		"\u00d1\u00d2\3\2\2\2\u00d2\u00d4\3\2\2\2\u00d3\u00d1\3\2\2\2\u00d4\u00d5"+
		"\7\22\2\2\u00d5\35\3\2\2\2\u00d6\u00d9\5\4\3\2\u00d7\u00d9\5 \21\2\u00d8"+
		"\u00d6\3\2\2\2\u00d8\u00d7\3\2\2\2\u00d9\37\3\2\2\2\u00da\u00db\5&\24"+
		"\2\u00db\u00dc\7\3\2\2\u00dc\u00dd\5\"\22\2\u00dd\u00de\7\24\2\2\u00de"+
		"\u0102\3\2\2\2\u00df\u00e1\5\"\22\2\u00e0\u00df\3\2\2\2\u00e0\u00e1\3"+
		"\2\2\2\u00e1\u00e2\3\2\2\2\u00e2\u0102\7\24\2\2\u00e3\u0102\5\34\17\2"+
		"\u00e4\u00e5\7\b\2\2\u00e5\u00e6\7\r\2\2\u00e6\u00e7\5$\23\2\u00e7\u00e8"+
		"\7\16\2\2\u00e8\u00e9\5 \21\2\u00e9\u0102\3\2\2\2\u00ea\u00eb\7\b\2\2"+
		"\u00eb\u00ec\7\r\2\2\u00ec\u00ed\5$\23\2\u00ed\u00ee\7\16\2\2\u00ee\u00ef"+
		"\5 \21\2\u00ef\u00f0\7\t\2\2\u00f0\u00f1\5 \21\2\u00f1\u0102\3\2\2\2\u00f2"+
		"\u00f3\7\n\2\2\u00f3\u00f4\7\r\2\2\u00f4\u00f5\5$\23\2\u00f5\u00f6\7\16"+
		"\2\2\u00f6\u00f7\5 \21\2\u00f7\u0102\3\2\2\2\u00f8\u00f9\7\13\2\2\u00f9"+
		"\u0102\7\24\2\2\u00fa\u00fb\7\f\2\2\u00fb\u0102\7\24\2\2\u00fc\u00fe\7"+
		"\7\2\2\u00fd\u00ff\5\"\22\2\u00fe\u00fd\3\2\2\2\u00fe\u00ff\3\2\2\2\u00ff"+
		"\u0100\3\2\2\2\u0100\u0102\7\24\2\2\u0101\u00da\3\2\2\2\u0101\u00e0\3"+
		"\2\2\2\u0101\u00e3\3\2\2\2\u0101\u00e4\3\2\2\2\u0101\u00ea\3\2\2\2\u0101"+
		"\u00f2\3\2\2\2\u0101\u00f8\3\2\2\2\u0101\u00fa\3\2\2\2\u0101\u00fc\3\2"+
		"\2\2\u0102!\3\2\2\2\u0103\u0104\5\66\34\2\u0104#\3\2\2\2\u0105\u0106\5"+
		"> \2\u0106%\3\2\2\2\u0107\u010e\7$\2\2\u0108\u0109\7\17\2\2\u0109\u010a"+
		"\5\"\22\2\u010a\u010b\7\20\2\2\u010b\u010d\3\2\2\2\u010c\u0108\3\2\2\2"+
		"\u010d\u0110\3\2\2\2\u010e\u010c\3\2\2\2\u010e\u010f\3\2\2\2\u010f\'\3"+
		"\2\2\2\u0110\u010e\3\2\2\2\u0111\u0112\7\r\2\2\u0112\u0113\5\"\22\2\u0113"+
		"\u0114\7\16\2\2\u0114\u0118\3\2\2\2\u0115\u0118\5&\24\2\u0116\u0118\5"+
		"*\26\2\u0117\u0111\3\2\2\2\u0117\u0115\3\2\2\2\u0117\u0116\3\2\2\2\u0118"+
		")\3\2\2\2\u0119\u011a\7#\2\2\u011a+\3\2\2\2\u011b\u0126\5(\25\2\u011c"+
		"\u011d\7$\2\2\u011d\u011f\7\r\2\2\u011e\u0120\5\60\31\2\u011f\u011e\3"+
		"\2\2\2\u011f\u0120\3\2\2\2\u0120\u0121\3\2\2\2\u0121\u0126\7\16\2\2\u0122"+
		"\u0123\5.\30\2\u0123\u0124\5,\27\2\u0124\u0126\3\2\2\2\u0125\u011b\3\2"+
		"\2\2\u0125\u011c\3\2\2\2\u0125\u0122\3\2\2\2\u0126-\3\2\2\2\u0127\u0128"+
		"\t\3\2\2\u0128/\3\2\2\2\u0129\u012e\5\62\32\2\u012a\u012b\7\23\2\2\u012b"+
		"\u012d\5\62\32\2\u012c\u012a\3\2\2\2\u012d\u0130\3\2\2\2\u012e\u012c\3"+
		"\2\2\2\u012e\u012f\3\2\2\2\u012f\61\3\2\2\2\u0130\u012e\3\2\2\2\u0131"+
		"\u0132\5\"\22\2\u0132\63\3\2\2\2\u0133\u0134\b\33\1\2\u0134\u0135\5,\27"+
		"\2\u0135\u013b\3\2\2\2\u0136\u0137\f\3\2\2\u0137\u0138\t\4\2\2\u0138\u013a"+
		"\5,\27\2\u0139\u0136\3\2\2\2\u013a\u013d\3\2\2\2\u013b\u0139\3\2\2\2\u013b"+
		"\u013c\3\2\2\2\u013c\65\3\2\2\2\u013d\u013b\3\2\2\2\u013e\u013f\b\34\1"+
		"\2\u013f\u0140\5\64\33\2\u0140\u0146\3\2\2\2\u0141\u0142\f\3\2\2\u0142"+
		"\u0143\t\5\2\2\u0143\u0145\5\64\33\2\u0144\u0141\3\2\2\2\u0145\u0148\3"+
		"\2\2\2\u0146\u0144\3\2\2\2\u0146\u0147\3\2\2\2\u0147\67\3\2\2\2\u0148"+
		"\u0146\3\2\2\2\u0149\u014a\b\35\1\2\u014a\u014b\5\66\34\2\u014b\u0151"+
		"\3\2\2\2\u014c\u014d\f\3\2\2\u014d\u014e\t\6\2\2\u014e\u0150\5\66\34\2"+
		"\u014f\u014c\3\2\2\2\u0150\u0153\3\2\2\2\u0151\u014f\3\2\2\2\u0151\u0152"+
		"\3\2\2\2\u01529\3\2\2\2\u0153\u0151\3\2\2\2\u0154\u0155\b\36\1\2\u0155"+
		"\u0156\58\35\2\u0156\u015c\3\2\2\2\u0157\u0158\f\3\2\2\u0158\u0159\t\7"+
		"\2\2\u0159\u015b\58\35\2\u015a\u0157\3\2\2\2\u015b\u015e\3\2\2\2\u015c"+
		"\u015a\3\2\2\2\u015c\u015d\3\2\2\2\u015d;\3\2\2\2\u015e\u015c\3\2\2\2"+
		"\u015f\u0160\b\37\1\2\u0160\u0161\5:\36\2\u0161\u0167\3\2\2\2\u0162\u0163"+
		"\f\3\2\2\u0163\u0164\7\33\2\2\u0164\u0166\5:\36\2\u0165\u0162\3\2\2\2"+
		"\u0166\u0169\3\2\2\2\u0167\u0165\3\2\2\2\u0167\u0168\3\2\2\2\u0168=\3"+
		"\2\2\2\u0169\u0167\3\2\2\2\u016a\u016b\b \1\2\u016b\u016c\5<\37\2\u016c"+
		"\u0172\3\2\2\2\u016d\u016e\f\3\2\2\u016e\u016f\7\34\2\2\u016f\u0171\5"+
		"<\37\2\u0170\u016d\3\2\2\2\u0171\u0174\3\2\2\2\u0172\u0170\3\2\2\2\u0172"+
		"\u0173\3\2\2\2\u0173?\3\2\2\2\u0174\u0172\3\2\2\2\u0175\u0176\5\66\34"+
		"\2\u0176A\3\2\2\2%DFMVdqtw\177\u008b\u0095\u009a\u00a3\u00a6\u00a9\u00af"+
		"\u00bb\u00c8\u00cb\u00d1\u00d8\u00e0\u00fe\u0101\u010e\u0117\u011f\u0125"+
		"\u012e\u013b\u0146\u0151\u015c\u0167\u0172";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}