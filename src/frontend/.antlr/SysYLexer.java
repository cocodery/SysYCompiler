// Generated from /home/why/workspace/Compiler/MyCompiler/SysYCompiler/src/frontend/SysY.g4 by ANTLR 4.8
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class SysYLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, Int=4, Void=5, Const=6, Return=7, If=8, Else=9, 
		While=10, Break=11, Continue=12, Lparen=13, Rparen=14, Lbrkt=15, Rbrkt=16, 
		Lbrace=17, Rbrace=18, Comma=19, Semicolon=20, Minus=21, Addition=22, Exclamation=23, 
		Multiplication=24, Division=25, Modulo=26, LAND=27, LOR=28, EQ=29, NEQ=30, 
		LT=31, LE=32, GT=33, GE=34, IntLiteral=35, Identifier=36, WS=37, LINE_COMMENT=38, 
		COMMENT=39;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "Int", "Void", "Const", "Return", "If", "Else", 
			"While", "Break", "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", 
			"Rbrace", "Comma", "Semicolon", "Minus", "Addition", "Exclamation", "Multiplication", 
			"Division", "Modulo", "LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", 
			"IntLiteral", "Identifier", "WS", "LINE_COMMENT", "COMMENT"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'='", "'for'", "'do'", "'int'", "'void'", "'const'", "'return'", 
			"'if'", "'else'", "'while'", "'break'", "'continue'", "'('", "')'", "'['", 
			"']'", "'{'", "'}'", "','", "';'", "'-'", "'+'", "'!'", "'*'", "'/'", 
			"'%'", "'&&'", "'||'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, "Int", "Void", "Const", "Return", "If", "Else", 
			"While", "Break", "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", 
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


	public SysYLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "SysY.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2)\u00ff\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\3\2\3\2\3\3\3\3\3\3"+
		"\3\3\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\6\3\7\3\7\3\7\3\7\3"+
		"\7\3\7\3\b\3\b\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\n\3\n\3\n\3\n\3\n\3\13"+
		"\3\13\3\13\3\13\3\13\3\13\3\f\3\f\3\f\3\f\3\f\3\f\3\r\3\r\3\r\3\r\3\r"+
		"\3\r\3\r\3\r\3\r\3\16\3\16\3\17\3\17\3\20\3\20\3\21\3\21\3\22\3\22\3\23"+
		"\3\23\3\24\3\24\3\25\3\25\3\26\3\26\3\27\3\27\3\30\3\30\3\31\3\31\3\32"+
		"\3\32\3\33\3\33\3\34\3\34\3\34\3\35\3\35\3\35\3\36\3\36\3\36\3\37\3\37"+
		"\3\37\3 \3 \3!\3!\3!\3\"\3\"\3#\3#\3#\3$\6$\u00c1\n$\r$\16$\u00c2\3$\3"+
		"$\3$\3$\6$\u00c9\n$\r$\16$\u00ca\3$\3$\3$\3$\6$\u00d1\n$\r$\16$\u00d2"+
		"\5$\u00d5\n$\3%\3%\7%\u00d9\n%\f%\16%\u00dc\13%\3&\3&\3&\3&\3\'\3\'\3"+
		"\'\3\'\7\'\u00e6\n\'\f\'\16\'\u00e9\13\'\3\'\5\'\u00ec\n\'\3\'\3\'\3\'"+
		"\3\'\3(\3(\3(\3(\7(\u00f6\n(\f(\16(\u00f9\13(\3(\3(\3(\3(\3(\4\u00e7\u00f7"+
		"\2)\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35"+
		"\20\37\21!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63\33\65\34\67\359\36"+
		";\37= ?!A\"C#E$G%I&K\'M(O)\3\2\7\3\2\62;\5\2\62;CHch\5\2C\\aac|\6\2\62"+
		";C\\aac|\5\2\13\f\17\17\"\"\2\u0107\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2"+
		"\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23"+
		"\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2"+
		"\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2"+
		"\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3"+
		"\2\2\2\2\67\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2"+
		"\2\2\2C\3\2\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2\2\2K\3\2\2\2\2M\3\2\2\2"+
		"\2O\3\2\2\2\3Q\3\2\2\2\5S\3\2\2\2\7W\3\2\2\2\tZ\3\2\2\2\13^\3\2\2\2\r"+
		"c\3\2\2\2\17i\3\2\2\2\21p\3\2\2\2\23s\3\2\2\2\25x\3\2\2\2\27~\3\2\2\2"+
		"\31\u0084\3\2\2\2\33\u008d\3\2\2\2\35\u008f\3\2\2\2\37\u0091\3\2\2\2!"+
		"\u0093\3\2\2\2#\u0095\3\2\2\2%\u0097\3\2\2\2\'\u0099\3\2\2\2)\u009b\3"+
		"\2\2\2+\u009d\3\2\2\2-\u009f\3\2\2\2/\u00a1\3\2\2\2\61\u00a3\3\2\2\2\63"+
		"\u00a5\3\2\2\2\65\u00a7\3\2\2\2\67\u00a9\3\2\2\29\u00ac\3\2\2\2;\u00af"+
		"\3\2\2\2=\u00b2\3\2\2\2?\u00b5\3\2\2\2A\u00b7\3\2\2\2C\u00ba\3\2\2\2E"+
		"\u00bc\3\2\2\2G\u00d4\3\2\2\2I\u00d6\3\2\2\2K\u00dd\3\2\2\2M\u00e1\3\2"+
		"\2\2O\u00f1\3\2\2\2QR\7?\2\2R\4\3\2\2\2ST\7h\2\2TU\7q\2\2UV\7t\2\2V\6"+
		"\3\2\2\2WX\7f\2\2XY\7q\2\2Y\b\3\2\2\2Z[\7k\2\2[\\\7p\2\2\\]\7v\2\2]\n"+
		"\3\2\2\2^_\7x\2\2_`\7q\2\2`a\7k\2\2ab\7f\2\2b\f\3\2\2\2cd\7e\2\2de\7q"+
		"\2\2ef\7p\2\2fg\7u\2\2gh\7v\2\2h\16\3\2\2\2ij\7t\2\2jk\7g\2\2kl\7v\2\2"+
		"lm\7w\2\2mn\7t\2\2no\7p\2\2o\20\3\2\2\2pq\7k\2\2qr\7h\2\2r\22\3\2\2\2"+
		"st\7g\2\2tu\7n\2\2uv\7u\2\2vw\7g\2\2w\24\3\2\2\2xy\7y\2\2yz\7j\2\2z{\7"+
		"k\2\2{|\7n\2\2|}\7g\2\2}\26\3\2\2\2~\177\7d\2\2\177\u0080\7t\2\2\u0080"+
		"\u0081\7g\2\2\u0081\u0082\7c\2\2\u0082\u0083\7m\2\2\u0083\30\3\2\2\2\u0084"+
		"\u0085\7e\2\2\u0085\u0086\7q\2\2\u0086\u0087\7p\2\2\u0087\u0088\7v\2\2"+
		"\u0088\u0089\7k\2\2\u0089\u008a\7p\2\2\u008a\u008b\7w\2\2\u008b\u008c"+
		"\7g\2\2\u008c\32\3\2\2\2\u008d\u008e\7*\2\2\u008e\34\3\2\2\2\u008f\u0090"+
		"\7+\2\2\u0090\36\3\2\2\2\u0091\u0092\7]\2\2\u0092 \3\2\2\2\u0093\u0094"+
		"\7_\2\2\u0094\"\3\2\2\2\u0095\u0096\7}\2\2\u0096$\3\2\2\2\u0097\u0098"+
		"\7\177\2\2\u0098&\3\2\2\2\u0099\u009a\7.\2\2\u009a(\3\2\2\2\u009b\u009c"+
		"\7=\2\2\u009c*\3\2\2\2\u009d\u009e\7/\2\2\u009e,\3\2\2\2\u009f\u00a0\7"+
		"-\2\2\u00a0.\3\2\2\2\u00a1\u00a2\7#\2\2\u00a2\60\3\2\2\2\u00a3\u00a4\7"+
		",\2\2\u00a4\62\3\2\2\2\u00a5\u00a6\7\61\2\2\u00a6\64\3\2\2\2\u00a7\u00a8"+
		"\7\'\2\2\u00a8\66\3\2\2\2\u00a9\u00aa\7(\2\2\u00aa\u00ab\7(\2\2\u00ab"+
		"8\3\2\2\2\u00ac\u00ad\7~\2\2\u00ad\u00ae\7~\2\2\u00ae:\3\2\2\2\u00af\u00b0"+
		"\7?\2\2\u00b0\u00b1\7?\2\2\u00b1<\3\2\2\2\u00b2\u00b3\7#\2\2\u00b3\u00b4"+
		"\7?\2\2\u00b4>\3\2\2\2\u00b5\u00b6\7>\2\2\u00b6@\3\2\2\2\u00b7\u00b8\7"+
		">\2\2\u00b8\u00b9\7?\2\2\u00b9B\3\2\2\2\u00ba\u00bb\7@\2\2\u00bbD\3\2"+
		"\2\2\u00bc\u00bd\7@\2\2\u00bd\u00be\7?\2\2\u00beF\3\2\2\2\u00bf\u00c1"+
		"\t\2\2\2\u00c0\u00bf\3\2\2\2\u00c1\u00c2\3\2\2\2\u00c2\u00c0\3\2\2\2\u00c2"+
		"\u00c3\3\2\2\2\u00c3\u00d5\3\2\2\2\u00c4\u00c5\7\62\2\2\u00c5\u00c6\7"+
		"z\2\2\u00c6\u00c8\3\2\2\2\u00c7\u00c9\t\3\2\2\u00c8\u00c7\3\2\2\2\u00c9"+
		"\u00ca\3\2\2\2\u00ca\u00c8\3\2\2\2\u00ca\u00cb\3\2\2\2\u00cb\u00d5\3\2"+
		"\2\2\u00cc\u00cd\7\62\2\2\u00cd\u00ce\7Z\2\2\u00ce\u00d0\3\2\2\2\u00cf"+
		"\u00d1\t\3\2\2\u00d0\u00cf\3\2\2\2\u00d1\u00d2\3\2\2\2\u00d2\u00d0\3\2"+
		"\2\2\u00d2\u00d3\3\2\2\2\u00d3\u00d5\3\2\2\2\u00d4\u00c0\3\2\2\2\u00d4"+
		"\u00c4\3\2\2\2\u00d4\u00cc\3\2\2\2\u00d5H\3\2\2\2\u00d6\u00da\t\4\2\2"+
		"\u00d7\u00d9\t\5\2\2\u00d8\u00d7\3\2\2\2\u00d9\u00dc\3\2\2\2\u00da\u00d8"+
		"\3\2\2\2\u00da\u00db\3\2\2\2\u00dbJ\3\2\2\2\u00dc\u00da\3\2\2\2\u00dd"+
		"\u00de\t\6\2\2\u00de\u00df\3\2\2\2\u00df\u00e0\b&\2\2\u00e0L\3\2\2\2\u00e1"+
		"\u00e2\7\61\2\2\u00e2\u00e3\7\61\2\2\u00e3\u00e7\3\2\2\2\u00e4\u00e6\13"+
		"\2\2\2\u00e5\u00e4\3\2\2\2\u00e6\u00e9\3\2\2\2\u00e7\u00e8\3\2\2\2\u00e7"+
		"\u00e5\3\2\2\2\u00e8\u00eb\3\2\2\2\u00e9\u00e7\3\2\2\2\u00ea\u00ec\7\17"+
		"\2\2\u00eb\u00ea\3\2\2\2\u00eb\u00ec\3\2\2\2\u00ec\u00ed\3\2\2\2\u00ed"+
		"\u00ee\7\f\2\2\u00ee\u00ef\3\2\2\2\u00ef\u00f0\b\'\2\2\u00f0N\3\2\2\2"+
		"\u00f1\u00f2\7\61\2\2\u00f2\u00f3\7,\2\2\u00f3\u00f7\3\2\2\2\u00f4\u00f6"+
		"\13\2\2\2\u00f5\u00f4\3\2\2\2\u00f6\u00f9\3\2\2\2\u00f7\u00f8\3\2\2\2"+
		"\u00f7\u00f5\3\2\2\2\u00f8\u00fa\3\2\2\2\u00f9\u00f7\3\2\2\2\u00fa\u00fb"+
		"\7,\2\2\u00fb\u00fc\7\61\2\2\u00fc\u00fd\3\2\2\2\u00fd\u00fe\b(\2\2\u00fe"+
		"P\3\2\2\2\13\2\u00c2\u00ca\u00d2\u00d4\u00da\u00e7\u00eb\u00f7\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}