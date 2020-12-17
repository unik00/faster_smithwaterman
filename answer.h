//
// Created by huynd107 on 15/12/2020.
//

#ifndef OPT_LEVEN_ANSWER_H
#define OPT_LEVEN_ANSWER_H

struct Answer{
    int score;
    std::pair<int, int> first_substring;
    std::pair<int, int> second_substring;
    bool operator==(Answer b) const{
        return score == b.score && first_substring == b.first_substring && second_substring == b.second_substring;
    }
    bool operator!=(Answer b) const{
        return score != b.score || first_substring != b.first_substring || second_substring != b.second_substring;

    }
};

#endif //OPT_LEVEN_ANSWER_H
