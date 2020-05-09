#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class SyntaxHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStart;
    QRegularExpression commentEnd;

    QTextCharFormat keyword;
    QTextCharFormat singleLineComment;
    QTextCharFormat quotation;
    QTextCharFormat function;
};

#endif // SYNTAXHIGHLIGHTER_H
