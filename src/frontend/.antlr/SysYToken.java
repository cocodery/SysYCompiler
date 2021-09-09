// Generated from /home/why/SysYCompiler/src/frontend/SysYToken.g4 by ANTLR 4.8
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class SysYToken extends Lexer {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		Int=1, Void=2, Const=3, Return=4, If=5, Else=6, While=7, Break=8, Continue=9, 
		Lparen=10, Rparen=11, Lbrkt=12, Rbrkt=13, Lbrace=14, Rbrace=15, Comma=16, 
		Semicolon=17, Minus=18, Addition=19, Exclamation=20, Multiplication=21, 
		Division=22, Modulo=23, LAND=24, LOR=25, EQ=26, NEQ=27, LT=28, LE=29, 
		GT=30, GE=31, IntLiteral=32, Identifier=33, WS=34, LINE_COMMENT=35, COMMENT=36;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"Int", "Void", "Const", "Return", "If", "Else", "While", "Break", "Continue", 
			"Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", "Rbrace", "Comma", "Semicolon", 
			"Minus", "Addition", "Exclamation", "Multiplication", "Division", "Modulo", 
			"LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", "IntLiteral", "Identifier", 
			"WS", "LINE_COMMENT", "COMMENT"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'int'", "'void'", "'const'", "'return'", "'if'", "'else'", "'while'", 
			"'break'", "'continue'", "'('", "')'", "'['", "']'", "'{'", "'}'", "','", 
			"';'", "'-'", "'+'", "'!'", "'*'", "'/'", "'%'", "'&&'", "'||'", "'=='", 
			"'!='", "'<'", "'<='", "'>'", "'>='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "Int", "Void", "Const", "Return", "If", "Else", "While", "Break", 
			"Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", "Rbrace", 
			"Comma", "Semicolon", "Minus", "Addition", "Exclamation", "Multiplication", 
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


	public SysYToken(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "SysYToken.g4"; }

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
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2&\u00f0\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\4"+
		"\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\7\3\7\3"+
		"\7\3\7\3\7\3\b\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\t\3\n\3\n\3\n"+
		"\3\n\3\n\3\n\3\n\3\n\3\n\3\13\3\13\3\f\3\f\3\r\3\r\3\16\3\16\3\17\3\17"+
		"\3\20\3\20\3\21\3\21\3\22\3\22\3\23\3\23\3\24\3\24\3\25\3\25\3\26\3\26"+
		"\3\27\3\27\3\30\3\30\3\31\3\31\3\31\3\32\3\32\3\32\3\33\3\33\3\33\3\34"+
		"\3\34\3\34\3\35\3\35\3\36\3\36\3\36\3\37\3\37\3 \3 \3 \3!\6!\u00b2\n!"+
		"\r!\16!\u00b3\3!\3!\3!\3!\6!\u00ba\n!\r!\16!\u00bb\3!\3!\3!\3!\6!\u00c2"+
		"\n!\r!\16!\u00c3\5!\u00c6\n!\3\"\3\"\7\"\u00ca\n\"\f\"\16\"\u00cd\13\""+
		"\3#\3#\3#\3#\3$\3$\3$\3$\7$\u00d7\n$\f$\16$\u00da\13$\3$\5$\u00dd\n$\3"+
		"$\3$\3$\3$\3%\3%\3%\3%\7%\u00e7\n%\f%\16%\u00ea\13%\3%\3%\3%\3%\3%\4\u00d8"+
		"\u00e8\2&\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33"+
		"\17\35\20\37\21!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63\33\65\34\67"+
		"\359\36;\37= ?!A\"C#E$G%I&\3\2\7\3\2\62;\5\2\62;CHch\5\2C\\aac|\6\2\62"+
		";C\\aac|\5\2\13\f\17\17\"\"\2\u00f8\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2"+
		"\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23"+
		"\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2"+
		"\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2"+
		"\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3"+
		"\2\2\2\2\67\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2"+
		"\2\2\2C\3\2\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2\2\3K\3\2\2\2\5O\3\2\2\2"+
		"\7T\3\2\2\2\tZ\3\2\2\2\13a\3\2\2\2\rd\3\2\2\2\17i\3\2\2\2\21o\3\2\2\2"+
		"\23u\3\2\2\2\25~\3\2\2\2\27\u0080\3\2\2\2\31\u0082\3\2\2\2\33\u0084\3"+
		"\2\2\2\35\u0086\3\2\2\2\37\u0088\3\2\2\2!\u008a\3\2\2\2#\u008c\3\2\2\2"+
		"%\u008e\3\2\2\2\'\u0090\3\2\2\2)\u0092\3\2\2\2+\u0094\3\2\2\2-\u0096\3"+
		"\2\2\2/\u0098\3\2\2\2\61\u009a\3\2\2\2\63\u009d\3\2\2\2\65\u00a0\3\2\2"+
		"\2\67\u00a3\3\2\2\29\u00a6\3\2\2\2;\u00a8\3\2\2\2=\u00ab\3\2\2\2?\u00ad"+
		"\3\2\2\2A\u00c5\3\2\2\2C\u00c7\3\2\2\2E\u00ce\3\2\2\2G\u00d2\3\2\2\2I"+
		"\u00e2\3\2\2\2KL\7k\2\2LM\7p\2\2MN\7v\2\2N\4\3\2\2\2OP\7x\2\2PQ\7q\2\2"+
		"QR\7k\2\2RS\7f\2\2S\6\3\2\2\2TU\7e\2\2UV\7q\2\2VW\7p\2\2WX\7u\2\2XY\7"+
		"v\2\2Y\b\3\2\2\2Z[\7t\2\2[\\\7g\2\2\\]\7v\2\2]^\7w\2\2^_\7t\2\2_`\7p\2"+
		"\2`\n\3\2\2\2ab\7k\2\2bc\7h\2\2c\f\3\2\2\2de\7g\2\2ef\7n\2\2fg\7u\2\2"+
		"gh\7g\2\2h\16\3\2\2\2ij\7y\2\2jk\7j\2\2kl\7k\2\2lm\7n\2\2mn\7g\2\2n\20"+
		"\3\2\2\2op\7d\2\2pq\7t\2\2qr\7g\2\2rs\7c\2\2st\7m\2\2t\22\3\2\2\2uv\7"+
		"e\2\2vw\7q\2\2wx\7p\2\2xy\7v\2\2yz\7k\2\2z{\7p\2\2{|\7w\2\2|}\7g\2\2}"+
		"\24\3\2\2\2~\177\7*\2\2\177\26\3\2\2\2\u0080\u0081\7+\2\2\u0081\30\3\2"+
		"\2\2\u0082\u0083\7]\2\2\u0083\32\3\2\2\2\u0084\u0085\7_\2\2\u0085\34\3"+
		"\2\2\2\u0086\u0087\7}\2\2\u0087\36\3\2\2\2\u0088\u0089\7\177\2\2\u0089"+
		" \3\2\2\2\u008a\u008b\7.\2\2\u008b\"\3\2\2\2\u008c\u008d\7=\2\2\u008d"+
		"$\3\2\2\2\u008e\u008f\7/\2\2\u008f&\3\2\2\2\u0090\u0091\7-\2\2\u0091("+
		"\3\2\2\2\u0092\u0093\7#\2\2\u0093*\3\2\2\2\u0094\u0095\7,\2\2\u0095,\3"+
		"\2\2\2\u0096\u0097\7\61\2\2\u0097.\3\2\2\2\u0098\u0099\7\'\2\2\u0099\60"+
		"\3\2\2\2\u009a\u009b\7(\2\2\u009b\u009c\7(\2\2\u009c\62\3\2\2\2\u009d"+
		"\u009e\7~\2\2\u009e\u009f\7~\2\2\u009f\64\3\2\2\2\u00a0\u00a1\7?\2\2\u00a1"+
		"\u00a2\7?\2\2\u00a2\66\3\2\2\2\u00a3\u00a4\7#\2\2\u00a4\u00a5\7?\2\2\u00a5"+
		"8\3\2\2\2\u00a6\u00a7\7>\2\2\u00a7:\3\2\2\2\u00a8\u00a9\7>\2\2\u00a9\u00aa"+
		"\7?\2\2\u00aa<\3\2\2\2\u00ab\u00ac\7@\2\2\u00ac>\3\2\2\2\u00ad\u00ae\7"+
		"@\2\2\u00ae\u00af\7?\2\2\u00af@\3\2\2\2\u00b0\u00b2\t\2\2\2\u00b1\u00b0"+
		"\3\2\2\2\u00b2\u00b3\3\2\2\2\u00b3\u00b1\3\2\2\2\u00b3\u00b4\3\2\2\2\u00b4"+
		"\u00c6\3\2\2\2\u00b5\u00b6\7\62\2\2\u00b6\u00b7\7z\2\2\u00b7\u00b9\3\2"+
		"\2\2\u00b8\u00ba\t\3\2\2\u00b9\u00b8\3\2\2\2\u00ba\u00bb\3\2\2\2\u00bb"+
		"\u00b9\3\2\2\2\u00bb\u00bc\3\2\2\2\u00bc\u00c6\3\2\2\2\u00bd\u00be\7\62"+
		"\2\2\u00be\u00bf\7Z\2\2\u00bf\u00c1\3\2\2\2\u00c0\u00c2\t\3\2\2\u00c1"+
		"\u00c0\3\2\2\2\u00c2\u00c3\3\2\2\2\u00c3\u00c1\3\2\2\2\u00c3\u00c4\3\2"+
		"\2\2\u00c4\u00c6\3\2\2\2\u00c5\u00b1\3\2\2\2\u00c5\u00b5\3\2\2\2\u00c5"+
		"\u00bd\3\2\2\2\u00c6B\3\2\2\2\u00c7\u00cb\t\4\2\2\u00c8\u00ca\t\5\2\2"+
		"\u00c9\u00c8\3\2\2\2\u00ca\u00cd\3\2\2\2\u00cb\u00c9\3\2\2\2\u00cb\u00cc"+
		"\3\2\2\2\u00ccD\3\2\2\2\u00cd\u00cb\3\2\2\2\u00ce\u00cf\t\6\2\2\u00cf"+
		"\u00d0\3\2\2\2\u00d0\u00d1\b#\2\2\u00d1F\3\2\2\2\u00d2\u00d3\7\61\2\2"+
		"\u00d3\u00d4\7\61\2\2\u00d4\u00d8\3\2\2\2\u00d5\u00d7\13\2\2\2\u00d6\u00d5"+
		"\3\2\2\2\u00d7\u00da\3\2\2\2\u00d8\u00d9\3\2\2\2\u00d8\u00d6\3\2\2\2\u00d9"+
		"\u00dc\3\2\2\2\u00da\u00d8\3\2\2\2\u00db\u00dd\7\17\2\2\u00dc\u00db\3"+
		"\2\2\2\u00dc\u00dd\3\2\2\2\u00dd\u00de\3\2\2\2\u00de\u00df\7\f\2\2\u00df"+
		"\u00e0\3\2\2\2\u00e0\u00e1\b$\2\2\u00e1H\3\2\2\2\u00e2\u00e3\7\61\2\2"+
		"\u00e3\u00e4\7,\2\2\u00e4\u00e8\3\2\2\2\u00e5\u00e7\13\2\2\2\u00e6\u00e5"+
		"\3\2\2\2\u00e7\u00ea\3\2\2\2\u00e8\u00e9\3\2\2\2\u00e8\u00e6\3\2\2\2\u00e9"+
		"\u00eb\3\2\2\2\u00ea\u00e8\3\2\2\2\u00eb\u00ec\7,\2\2\u00ec\u00ed\7\61"+
		"\2\2\u00ed\u00ee\3\2\2\2\u00ee\u00ef\b%\2\2\u00efJ\3\2\2\2\13\2\u00b3"+
		"\u00bb\u00c3\u00c5\u00cb\u00d8\u00dc\u00e8\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}