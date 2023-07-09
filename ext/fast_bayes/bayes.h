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
    // amount of each label overall
    map<string, ll> priori_counts;
    // Total amount of observations recorded
    ll count = 0;
    // Total amounts of each term
    unordered_map<string, ll> term_counts;
    // Use english by default, TODO add option to use different ones
    const string default_stopwords = "ext/fast_bayes/stopwords/en";
    unordered_set<string> stopwords;

    double smoothing = 0.00000000000001;

    double estimate_priori(const string &label)
    {
        return (priori_counts[label] / (double)count);
    }

    // Occurences of term in class divided by sum of all occurences of term
    double estimate_term(const string &term, const string &label)
    {
        return (word_counts[label][term] + smoothing) / ((smoothing * term_counts.size()) + term_counts[term]);
    }

public:
    Bayes()
    {
        stopwords = read_stopwords(default_stopwords);
    }

    // Add a new observation
    void observe(const string &data, const string &label)
    {
        string ndata = data + " ";
        priori_counts[label]++;
        count++;

        int prev = 0;
        // Split by whitespace,
        for (unsigned int i = 0; i < ndata.length(); i++)
        {
            if (ndata[i] == ' ')
            {
                const string term = ndata.substr(prev, i - prev);

                if (term.length() < 2 || stopwords.find(term) != stopwords.end())
                    continue;
                word_counts[label][term]++;
                term_counts[term]++;
                prev = i + 1;
            }
        }
    }

    // want to edit this to return probabilities of each class ( can assume only 2 classes )
    // returns a string - prob easiest to just make a string vs. returning a hash?  "Good: 32342 Bad: 234234".
    string classify(const string &data)
    {
        string ndata = data + " ";
        string best_class = "";
        std::stringstream res;
        double best_score = -numeric_limits<double>::infinity();

        // for each classification GROUP
        for (const auto &label_pair : priori_counts)
        {
            const string label = label_pair.first;

            // Score for a single label given the data - this is it across all occurances.  Log of num words in corpus / total words?
            double score = log(estimate_priori(label));
            int prev = 0;
            // Split by whitespace,
            for (unsigned int i = 0; i < ndata.length(); i++)
            {
                if (ndata[i] == ' ')
                {
                    score += log(estimate_term(ndata.substr(prev, i - prev), label));
                    prev = i + 1;
                }
            }

            res << label << ":" << std::to_string(score) << " ";
        }
        return res.str();
    }
};

#endif
