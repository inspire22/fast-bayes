#ifndef bayes
#define bayes

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <limits>
#include <cmath>
#include "stopwords.h"
#include <sstream>

using namespace std;
typedef long long ll;

class Bayes
{

private:
    // For each label, the count of each word seen with that label
    map<string, unordered_map<string, ll>> word_counts;
    // For each label, the number of documents observed for it
    map<string, ll> doc_counts;
    // For each label, the total number of words observed for it
    map<string, ll> class_word_counts;
    // Total number of documents observed
    ll doc_count = 0;
    // All unique terms (the vocabulary) across all classes
    unordered_set<string> vocabulary;

    // Use english by default, TODO add option to use different ones
    const string default_stopwords = "ext/fast_bayes/stopwords/en";
    unordered_set<string> stopwords;

    double smoothing = 0.00000000000001;

    double estimate_priori(const string &label)
    {
        return (doc_counts[label] / (double)doc_count);
    }

    // Standard Multinomial Naive Bayes term probability with Laplace smoothing
    double estimate_term(const string &term, const string &label)
    {
        ll term_count_in_class = 0; // Default to 0 if term/label not seen
        if (word_counts.count(label) && word_counts[label].count(term))
        {
            term_count_in_class = word_counts[label][term];
        }

        ll total_words_in_class = class_word_counts[label];
        return (term_count_in_class + smoothing) / (total_words_in_class + (smoothing * vocabulary.size()));
    }

    // Tokenizer to be shared by observe and classify
    vector<pair<string, ll>> tokenize(const string &data)
    {
        vector<pair<string, ll>> tokens;
        string current_word;
        stringstream ss(data);

        while (ss >> current_word)
        {
            string term = current_word;
            ll occurrence_count = 1;

            size_t separator_pos = current_word.find("%%");
            if (separator_pos != string::npos)
            {
                term = current_word.substr(0, separator_pos);
                try
                {
                    occurrence_count = stoll(current_word.substr(separator_pos + 2));
                }
                catch (const std::invalid_argument &ia)
                {
                    // Ignore if parsing fails, treat as a single term with count 1
                    term = current_word;
                    occurrence_count = 1;
                }
            }

            if (term.length() >= 2 && stopwords.find(term) == stopwords.end())
            {
                tokens.push_back({term, occurrence_count});
            }
        }
        return tokens;
    }

public:
    Bayes()
    {
        stopwords = read_stopwords(default_stopwords);
    }

    // Add a new observation
    void observe(const string &data, const string &label)
    {
        doc_counts[label]++;
        doc_count++;

        vector<pair<string, ll>> tokens = tokenize(data);

        for (const auto &token_pair : tokens)
        {
            const string &term = token_pair.first;
            ll occurrence_count = token_pair.second;

            word_counts[label][term] += occurrence_count;
            class_word_counts[label] += occurrence_count;
            vocabulary.insert(term);
        }
    }

    // Returns a space-separated list of all labels and their log-probability scores
    string classify(const string &data)
    {
        stringstream result_stream;
        vector<pair<string, ll>> tokens = tokenize(data);

        for (const auto &label_pair : doc_counts)
        {
            const string label = label_pair.first;
            double score = log(estimate_priori(label));

            for (const auto &token_pair : tokens)
            {
                const string &term = token_pair.first;
                // Note: occurrence_count from tokenize is ignored during classification
                score += log(estimate_term(term, label));
            }

            result_stream << label << ":" << to_string(score) << " ";
        }

        string result = result_stream.str();
        if (!result.empty())
        {
            result.pop_back(); // Remove the trailing space
        }
        return result;
    }
};

#endif
