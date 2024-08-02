#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath> 

using namespace std;

// 四捨五入のための関数を設定する．値を最も近い100の倍数に四捨五入する．
double round100(double value) {
    return round(value / 100.0) * 100.0;
}

// 少数第1位まで表示するための関数を設定する．
string format(double value) {
    stringstream ss;
    ss << fixed << setprecision(1) << value; 
    return ss.str();
}

// Betクラスを作る． このクラスはオッズの管理，予算配分，投資収益率の計算などを行う．
class Bet {
private:
    vector<double> odds; 
    vector<double> betAmounts; 
    vector<double> payouts; 
    double totalBudget; 
    double totalRoundedBet; 
    double totalPayout; 

public:
    // 総予算とオッズのリストを受け取り，クラスのメンバー変数を初期化する．
    Bet(double budget, const vector<double>& oddsList)
        : totalBudget(budget), odds(oddsList), totalRoundedBet(0), totalPayout(0) {
        betAmounts.resize(odds.size());
        payouts.resize(odds.size());
        calculateBet();
    }

    // 予算の配分と配当額の計算する．
    void calculateBet() {
        double sumOdds = 0; 

        // 各オッズの逆数を計算し、その合計を求める．
        for (auto it = odds.begin(); it != odds.end(); ++it) {
            sumOdds += 1.0 / *it; 
        }

        // 各オッズに対する予算配分額と配当額を計算する．
        auto betIt = betAmounts.begin(); 
        auto payoutIt = payouts.begin(); 
        for (auto oddsIt = odds.begin(); oddsIt != odds.end(); ++oddsIt, ++betIt, ++payoutIt) {
            *betIt = (1.0 / *oddsIt) / sumOdds * totalBudget;
            *betIt = round100(*betIt);
            totalRoundedBet += *betIt;
            *payoutIt = *oddsIt * *betIt;
            totalPayout += *payoutIt;
        }
    }

    // 投資収益率を計算する．これは以下の式で求められる．
    // 投資収益率 = (配当額の合計 - 投入額の合計) / 投入額の合計 × 100
    double calculateROI() const {
        return (totalPayout - totalRoundedBet) / totalRoundedBet * 100;
    }

    // 結果を表示
    void displayResults() const {
        cout << "\n各オッズに対する予算分配額:\n";
        auto betIt = betAmounts.begin(); 
        auto payoutIt = payouts.begin(); 
        for (auto oddsIt = odds.begin(); oddsIt != odds.end(); ++oddsIt, ++betIt, ++payoutIt) {
            // 各オッズの分配額とその配当額を表示する．
            cout << "オッズ " << format(*oddsIt) << "倍: "
                << fixed << setprecision(0) << *betIt << " -> 当たった場合の配当額: "
                << fixed << setprecision(0) << *payoutIt << endl;
        }

        // 四捨五入後の合計予算額と合計配当額を表示する．
        cout << "\n実際の合計予算額: "
            << fixed << setprecision(0) << totalRoundedBet << endl;

        cout << "合計配当額: "
            << fixed << setprecision(0) << totalPayout << endl;

        // 投資収益率の評価
        double roi = calculateROI();
        cout << "投資収益率: "
            << fixed << setprecision(2) << roi << "%" << endl;

        // 投資収益率が100%を超えた場合のメッセージ表示
        if (roi > 100) {
            cout << "儲かる買い方です" << endl; // 100%を超える場合、利益が見込める
        }
        else {
            cout << "損する買い方です" << endl; // 100%未満の場合、損失が見込まれる
        }
    }
};

int main() {
    // ユーザーから予算の入力を受け取る
    double totalBudget;
    cout << "予算を入力してください（例：10000）: ";
    if (!(cin >> totalBudget)) {
        cerr << "予算の入力が無効です." << endl; // 入力エラーが発生した場合のメッセージ
        return 1; 
    }

    // ユーザーからオッズの入力を受け取る
    int numBets;
    cout << "予算を分配したいオッズの個数を1～99の範囲から入力してください (例:5): ";
    if (!(cin >> numBets) || numBets < 1 || numBets > 99) {
        cerr << "オッズの数は１から９９の間である必要があります." << endl; // オッズの数が範囲外の場合のメッセージ
        return 1; 
    }

    vector<double> odds; // オッズを格納するベクトル
    cout << "オッズの倍率をそれぞれ入力してください（2.5倍，3.0倍，4.9倍の計３つのオッズの場合の例：2.5 3.0 4.9）: ";
    odds.resize(numBets); 

    for (int i = 0; i < numBets; ++i) {
        if (!(cin >> odds[i])) {
            cerr << "オッズの入力が無効です." << endl; // オッズの入力エラー時のメッセージ
            return 1; 
        }
    }

    // Betオブジェクトを作成し、予算とオッズのリストを渡す
    Bet manager(totalBudget, odds);
    manager.displayResults();

    return 0; 
}