// Generated from /home/why/SysYCompiler/src/SysY.g4 by ANTLR 4.8
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
		T__0=1, Int=2, Void=3, Const=4, Return=5, If=6, Else=7, While=8, Break=9, 
		Continue=10, Lparen=11, Rparen=12, Lbrkt=13, Rbrkt=14, Lbrace=15, Rbrace=16, 
		Comma=17, Semicolon=18, Minus=19, Addition=20, Exclamation=21, Multiplication=22, 
		Division=23, Modulo=24, LAND=25, LOR=26, EQ=27, NEQ=28, LT=29, LE=30, 
		GT=31, GE=32, IntLiteral=33, Identifier=34, WS=35, LINE_COMMENT=36, COMMENT=37;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "Int", "Void", "Const", "Return", "If", "Else", "While", "Break", 
			"Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", "Rbrace", 
			"Comma", "Semicolon", "Minus", "Addition", "Exclamation", "Multiplication", 
			"Division", "Modulo", "LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", 
			"IntLiteral", "Identifier", "WS", "LINE_COMMENT", "COMMENT"
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
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\'\u00f4\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\3\2\3\2\3\3\3\3\3\3\3\3\3\4\3\4\3"+
		"\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\6\3\6\3\6\3\7\3\7"+
		"\3\7\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\t\3\n\3\n\3\n\3\n\3\n\3"+
		"\n\3\13\3\13\3\13\3\13\3\13\3\13\3\13\3\13\3\13\3\f\3\f\3\r\3\r\3\16\3"+
		"\16\3\17\3\17\3\20\3\20\3\21\3\21\3\22\3\22\3\23\3\23\3\24\3\24\3\25\3"+
		"\25\3\26\3\26\3\27\3\27\3\30\3\30\3\31\3\31\3\32\3\32\3\32\3\33\3\33\3"+
		"\33\3\34\3\34\3\34\3\35\3\35\3\35\3\36\3\36\3\37\3\37\3\37\3 \3 \3!\3"+
		"!\3!\3\"\6\"\u00b6\n\"\r\"\16\"\u00b7\3\"\3\"\3\"\3\"\6\"\u00be\n\"\r"+
		"\"\16\"\u00bf\3\"\3\"\3\"\3\"\6\"\u00c6\n\"\r\"\16\"\u00c7\5\"\u00ca\n"+
		"\"\3#\3#\7#\u00ce\n#\f#\16#\u00d1\13#\3$\3$\3$\3$\3%\3%\3%\3%\7%\u00db"+
		"\n%\f%\16%\u00de\13%\3%\5%\u00e1\n%\3%\3%\3%\3%\3&\3&\3&\3&\7&\u00eb\n"+
		"&\f&\16&\u00ee\13&\3&\3&\3&\3&\3&\4\u00dc\u00ec\2\'\3\3\5\4\7\5\t\6\13"+
		"\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35\20\37\21!\22#\23%\24\'"+
		"\25)\26+\27-\30/\31\61\32\63\33\65\34\67\359\36;\37= ?!A\"C#E$G%I&K\'"+
		"\3\2\7\3\2\62;\5\2\62;CHch\5\2C\\aac|\6\2\62;C\\aac|\5\2\13\f\17\17\""+
		"\"\2\u00fc\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2"+
		"\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27"+
		"\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2"+
		"\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2"+
		"\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3\2"+
		"\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2\2"+
		"\2G\3\2\2\2\2I\3\2\2\2\2K\3\2\2\2\3M\3\2\2\2\5O\3\2\2\2\7S\3\2\2\2\tX"+
		"\3\2\2\2\13^\3\2\2\2\re\3\2\2\2\17h\3\2\2\2\21m\3\2\2\2\23s\3\2\2\2\25"+
		"y\3\2\2\2\27\u0082\3\2\2\2\31\u0084\3\2\2\2\33\u0086\3\2\2\2\35\u0088"+
		"\3\2\2\2\37\u008a\3\2\2\2!\u008c\3\2\2\2#\u008e\3\2\2\2%\u0090\3\2\2\2"+
		"\'\u0092\3\2\2\2)\u0094\3\2\2\2+\u0096\3\2\2\2-\u0098\3\2\2\2/\u009a\3"+
		"\2\2\2\61\u009c\3\2\2\2\63\u009e\3\2\2\2\65\u00a1\3\2\2\2\67\u00a4\3\2"+
		"\2\29\u00a7\3\2\2\2;\u00aa\3\2\2\2=\u00ac\3\2\2\2?\u00af\3\2\2\2A\u00b1"+
		"\3\2\2\2C\u00c9\3\2\2\2E\u00cb\3\2\2\2G\u00d2\3\2\2\2I\u00d6\3\2\2\2K"+
		"\u00e6\3\2\2\2MN\7?\2\2N\4\3\2\2\2OP\7k\2\2PQ\7p\2\2QR\7v\2\2R\6\3\2\2"+
		"\2ST\7x\2\2TU\7q\2\2UV\7k\2\2VW\7f\2\2W\b\3\2\2\2XY\7e\2\2YZ\7q\2\2Z["+
		"\7p\2\2[\\\7u\2\2\\]\7v\2\2]\n\3\2\2\2^_\7t\2\2_`\7g\2\2`a\7v\2\2ab\7"+
		"w\2\2bc\7t\2\2cd\7p\2\2d\f\3\2\2\2ef\7k\2\2fg\7h\2\2g\16\3\2\2\2hi\7g"+
		"\2\2ij\7n\2\2jk\7u\2\2kl\7g\2\2l\20\3\2\2\2mn\7y\2\2no\7j\2\2op\7k\2\2"+
		"pq\7n\2\2qr\7g\2\2r\22\3\2\2\2st\7d\2\2tu\7t\2\2uv\7g\2\2vw\7c\2\2wx\7"+
		"m\2\2x\24\3\2\2\2yz\7e\2\2z{\7q\2\2{|\7p\2\2|}\7v\2\2}~\7k\2\2~\177\7"+
		"p\2\2\177\u0080\7w\2\2\u0080\u0081\7g\2\2\u0081\26\3\2\2\2\u0082\u0083"+
		"\7*\2\2\u0083\30\3\2\2\2\u0084\u0085\7+\2\2\u0085\32\3\2\2\2\u0086\u0087"+
		"\7]\2\2\u0087\34\3\2\2\2\u0088\u0089\7_\2\2\u0089\36\3\2\2\2\u008a\u008b"+
		"\7}\2\2\u008b \3\2\2\2\u008c\u008d\7\177\2\2\u008d\"\3\2\2\2\u008e\u008f"+
		"\7.\2\2\u008f$\3\2\2\2\u0090\u0091\7=\2\2\u0091&\3\2\2\2\u0092\u0093\7"+
		"/\2\2\u0093(\3\2\2\2\u0094\u0095\7-\2\2\u0095*\3\2\2\2\u0096\u0097\7#"+
		"\2\2\u0097,\3\2\2\2\u0098\u0099\7,\2\2\u0099.\3\2\2\2\u009a\u009b\7\61"+
		"\2\2\u009b\60\3\2\2\2\u009c\u009d\7\'\2\2\u009d\62\3\2\2\2\u009e\u009f"+
		"\7(\2\2\u009f\u00a0\7(\2\2\u00a0\64\3\2\2\2\u00a1\u00a2\7~\2\2\u00a2\u00a3"+
		"\7~\2\2\u00a3\66\3\2\2\2\u00a4\u00a5\7?\2\2\u00a5\u00a6\7?\2\2\u00a68"+
		"\3\2\2\2\u00a7\u00a8\7#\2\2\u00a8\u00a9\7?\2\2\u00a9:\3\2\2\2\u00aa\u00ab"+
		"\7>\2\2\u00ab<\3\2\2\2\u00ac\u00ad\7>\2\2\u00ad\u00ae\7?\2\2\u00ae>\3"+
		"\2\2\2\u00af\u00b0\7@\2\2\u00b0@\3\2\2\2\u00b1\u00b2\7@\2\2\u00b2\u00b3"+
		"\7?\2\2\u00b3B\3\2\2\2\u00b4\u00b6\t\2\2\2\u00b5\u00b4\3\2\2\2\u00b6\u00b7"+
		"\3\2\2\2\u00b7\u00b5\3\2\2\2\u00b7\u00b8\3\2\2\2\u00b8\u00ca\3\2\2\2\u00b9"+
		"\u00ba\7\62\2\2\u00ba\u00bb\7z\2\2\u00bb\u00bd\3\2\2\2\u00bc\u00be\t\3"+
		"\2\2\u00bd\u00bc\3\2\2\2\u00be\u00bf\3\2\2\2\u00bf\u00bd\3\2\2\2\u00bf"+
		"\u00c0\3\2\2\2\u00c0\u00ca\3\2\2\2\u00c1\u00c2\7\62\2\2\u00c2\u00c3\7"+
		"Z\2\2\u00c3\u00c5\3\2\2\2\u00c4\u00c6\t\3\2\2\u00c5\u00c4\3\2\2\2\u00c6"+
		"\u00c7\3\2\2\2\u00c7\u00c5\3\2\2\2\u00c7\u00c8\3\2\2\2\u00c8\u00ca\3\2"+
		"\2\2\u00c9\u00b5\3\2\2\2\u00c9\u00b9\3\2\2\2\u00c9\u00c1\3\2\2\2\u00ca"+
		"D\3\2\2\2\u00cb\u00cf\t\4\2\2\u00cc\u00ce\t\5\2\2\u00cd\u00cc\3\2\2\2"+
		"\u00ce\u00d1\3\2\2\2\u00cf\u00cd\3\2\2\2\u00cf\u00d0\3\2\2\2\u00d0F\3"+
		"\2\2\2\u00d1\u00cf\3\2\2\2\u00d2\u00d3\t\6\2\2\u00d3\u00d4\3\2\2\2\u00d4"+
		"\u00d5\b$\2\2\u00d5H\3\2\2\2\u00d6\u00d7\7\61\2\2\u00d7\u00d8\7\61\2\2"+
		"\u00d8\u00dc\3\2\2\2\u00d9\u00db\13\2\2\2\u00da\u00d9\3\2\2\2\u00db\u00de"+
		"\3\2\2\2\u00dc\u00dd\3\2\2\2\u00dc\u00da\3\2\2\2\u00dd\u00e0\3\2\2\2\u00de"+
		"\u00dc\3\2\2\2\u00df\u00e1\7\17\2\2\u00e0\u00df\3\2\2\2\u00e0\u00e1\3"+
		"\2\2\2\u00e1\u00e2\3\2\2\2\u00e2\u00e3\7\f\2\2\u00e3\u00e4\3\2\2\2\u00e4"+
		"\u00e5\b%\2\2\u00e5J\3\2\2\2\u00e6\u00e7\7\61\2\2\u00e7\u00e8\7,\2\2\u00e8"+
		"\u00ec\3\2\2\2\u00e9\u00eb\13\2\2\2\u00ea\u00e9\3\2\2\2\u00eb\u00ee\3"+
		"\2\2\2\u00ec\u00ed\3\2\2\2\u00ec\u00ea\3\2\2\2\u00ed\u00ef\3\2\2\2\u00ee"+
		"\u00ec\3\2\2\2\u00ef\u00f0\7,\2\2\u00f0\u00f1\7\61\2\2\u00f1\u00f2\3\2"+
		"\2\2\u00f2\u00f3\b&\2\2\u00f3L\3\2\2\2\13\2\u00b7\u00bf\u00c7\u00c9\u00cf"+
		"\u00dc\u00e0\u00ec\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}