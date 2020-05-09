#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent) : QSyntaxHighlighter {parent} {
    HighlightingRule rule;

    keyword.setForeground(Qt::magenta);
    keyword.setFontWeight(QFont::Bold);

    const QString keywordPatterns[] {
        QStringLiteral("\\bfor\\b"), QStringLiteral("\\bwhile\\b"),QStringLiteral("\\bin\\b"),
        QStringLiteral("\\bis\\b"),QStringLiteral("\\bas\\b"),QStringLiteral("\\bif\\b"),QStringLiteral("\\belif\\b"),
        QStringLiteral("\\belse\\b"),QStringLiteral("\\bimport\\b"),QStringLiteral("\\bfrom\\b"),
        QStringLiteral("\\bassert\\b"),QStringLiteral("\\bdef\\b"),QStringLiteral("\\bclass\\b"),
        QStringLiteral("\\bglobal\\b"),QStringLiteral("\\bTrue\\b"),QStringLiteral("\\bFalse\\b"),
        QStringLiteral("\\btry\\b"),QStringLiteral("\\bexcept\\b"),QStringLiteral("\\braise\\b"),
        QStringLiteral("\\bfinally\\b"),QStringLiteral("\\bpass\\b"),QStringLiteral("\\bbreak\\b"),
        QStringLiteral("\\bcontinue\\b"),QStringLiteral("\\breturn\\b"),QStringLiteral("\\bdel\\b"),
        QStringLiteral("\\blambda\\b"),QStringLiteral("\\bNone\\b"),QStringLiteral("\\bnonlocal\\b"),
        QStringLiteral("\\bwith\\b"),QStringLiteral("\\byield\\b"),QStringLiteral("\\band\\b"),
        QStringLiteral("\\bor\\b")
    };
    for(const QString& pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keyword;
        highlightingRules.append(rule);
    }

    number.setForeground(Qt::green);
    rule.pattern = QRegularExpression(QStringLiteral("[0-9.]"));
    rule.format = number;
    highlightingRules.append(rule);

    objectMethod.setForeground(Qt::magenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\.)"));
    rule.format = objectMethod;
    highlightingRules.append(rule);

    function.setFontItalic(true);
    function.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = function;
    highlightingRules.append(rule);

    quotation.setForeground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotation;
    highlightingRules.append(rule);

    charQuotation.setForeground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("'.*'"));
    rule.format = quotation;
    highlightingRules.append(rule);

    singleLineComment.setForeground(Qt::cyan);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineComment;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    for(const HighlightingRule& rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while(matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(),match.capturedLength(),rule.format);
        }
    }
}
