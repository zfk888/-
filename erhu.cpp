#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

// 十二平均律音名（国际音名）
const vector<string> chromatic_scale = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

// 二胡标准定弦
const string inner_string_note = "D";  // 内弦D
const string outer_string_note = "A";  // 外弦A

// 居中显示辅助函数
string center(const string& s, int width) {
    int pad = width - static_cast<int>(s.length());
    if (pad <= 0) return s;

    int pad_left = pad / 2;
    int pad_right = pad - pad_left;

    return string(pad_left, ' ') + s + string(pad_right, ' ');
}

// 获取音名对应的半音索引
int get_note_index(const string& note) {
    for (int i = 0; i < chromatic_scale.size(); i++) {
        if (chromatic_scale[i] == note) {
            return i;
        }
    }
    return -1;
}

// 根据根音计算简谱音阶 (1-7)
map<int, string> get_simplified_notation_scale(const string& root) {
    map<int, string> scale;

    // 找到根音位置
    int root_index = get_note_index(root);
    if (root_index == -1) return scale;

    // 大调音阶的半音间隔：全全半全全全半
    int intervals[] = { 0, 2, 4, 5, 7, 9, 11 };  // 相对于根音的偏移

    // 生成音阶
    for (int i = 0; i < 7; i++) {
        int note_index = (root_index + intervals[i]) % 12;
        string note_symbol;

        // 简谱表示
        if (i == 0) note_symbol = "1";
        else if (i == 1) note_symbol = "2";
        else if (i == 2) note_symbol = "3";
        else if (i == 3) note_symbol = "4";
        else if (i == 4) note_symbol = "5";
        else if (i == 5) note_symbol = "6";
        else if (i == 6) note_symbol = "7";

        scale[note_index] = note_symbol;
    }

    return scale;
}

// 处理升降号
string process_key_signature(const string& key) {
    string processed = key;

    // 处理降号
    if (key.length() > 1 && key[1] == 'b') {
        char base_note = key[0];
        if (base_note == 'B') return "A#";
        if (base_note == 'E') return "D#";
        if (base_note == 'A') return "G#";
        if (base_note == 'D') return "C#";
        if (base_note == 'G') return "F#";
        if (base_note == 'C') return "B";
        if (base_note == 'F') return "E";
    }

    return processed;
}

// 获取音名的简谱表示
string get_simplified_note(int note_index, const map<int, string>& scale) {
    if (scale.find(note_index) != scale.end()) {
        return scale.at(note_index);
    }
    else {
        // 检查是否是变化音
        string note_name = chromatic_scale[note_index];
        if (note_name.find('#') != string::npos) {
            // 找到基础音
            string base_note = note_name.substr(0, note_name.find('#'));
            int base_index = get_note_index(base_note);
            if (scale.find(base_index) != scale.end()) {
                return scale.at(base_index) + "#";
            }
        }
        else if (note_name.find('b') != string::npos) {
            // 对于降号表示
            string base_note = note_name.substr(0, note_name.find('b'));
            int base_index = get_note_index(base_note);
            if (scale.find(base_index) != scale.end()) {
                return scale.at(base_index) + "b";
            }
        }
        return "?";
    }
}

// 竖向打印二胡音位图
void print_erhu_fingering_chart_vertical(const string& key_signature) {
    string processed_key = process_key_signature(key_signature);
    cout << "\n┌─────────────────────────────────────────────┐" << endl;
    cout << "│            二胡音位图（竖向显示）           │" << endl;
    cout << "├─────────────────────────────────────────────┤" << endl;

    // 获取简谱音阶
    auto scale = get_simplified_notation_scale(processed_key);
    if (scale.empty()) {
        cout << "│             无效的调式！                  │" << endl;
        cout << "└─────────────────────────────────────────────┘" << endl;
        return;
    }

    // 显示调式信息
    cout << "│ 调式: " << setw(6) << left << key_signature << " (1=" << key_signature << ")"
        << setw(23) << right << "│" << endl;

    // 显示空弦信息
    int inner_index = get_note_index(inner_string_note);
    int outer_index = get_note_index(outer_string_note);

    string inner_string_display = get_simplified_note(inner_index, scale);
    string outer_string_display = get_simplified_note(outer_index, scale);

    cout << "│ 定弦: 内弦D(" << inner_string_display << ") 外弦A(" << outer_string_display << ")"
        << setw(12) << right << "│" << endl;
    cout << "├─────────────────────────────────────────────┤" << endl;

    // 定义把位和指法
    vector<string> positions = { "第一把位", "第二把位", "第三把位", "第四把位" };
    vector<string> fingers = { "空弦", "一指", "二指", "三指", "四指" };

    // 竖向显示表格
    cout << "│ " << setw(10) << left << "把位/指法"
        << setw(12) << "内弦音"
        << setw(12) << "外弦音"
        << "   │" << endl;
    cout << "├──────────┬────────────┬────────────┤" << endl;

    for (int pos = 0; pos < positions.size(); pos++) {
        // 显示把位标题
        if (pos > 0) {
            cout << "├──────────┼────────────┼────────────┤" << endl;
        }

        cout << "│ " << setw(8) << left << positions[pos] << " │"
            << setw(12) << center("", 12) << "│"
            << setw(12) << center("", 12) << "│" << endl;

        // 显示每个指位的音
        for (int finger = 0; finger < fingers.size(); finger++) {
            // 计算内弦音
            int inner_note_index;
            if (finger == 0) {
                // 空弦
                inner_note_index = (inner_index + pos * 7) % 12;
            }
            else {
                // 手指按音：每个把位内，手指按音递增
                // 简化的音程关系：1指=大二度，2指=大三/小三度，3指=纯四度，4指=纯五度
                int finger_interval[] = { 0, 2, 4, 5, 7 }; // 实际二胡音程
                inner_note_index = (inner_index + pos * 7 + finger_interval[finger]) % 12;
            }

            string inner_note = get_simplified_note(inner_note_index, scale);

            // 计算外弦音
            int outer_note_index;
            if (finger == 0) {
                // 空弦
                outer_note_index = (outer_index + pos * 7) % 12;
            }
            else {
                int finger_interval[] = { 0, 2, 4, 5, 7 }; // 实际二胡音程
                outer_note_index = (outer_index + pos * 7 + finger_interval[finger]) % 12;
            }

            string outer_note = get_simplified_note(outer_note_index, scale);

            cout << "│   " << setw(6) << left << fingers[finger]
                << " │ " << setw(10) << center(inner_note, 10)
                << " │ " << setw(10) << center(outer_note, 10)
                << " │" << endl;
        }
    }

    cout << "└──────────┴────────────┴────────────┘" << endl;

    // 显示特别提示
    cout << "\n【特别提示】" << endl;
    cout << "══════════════════════════════════════════════" << endl;

    if (key_signature == "D") {
        cout << "• D调(1=D): 空弦为1-5，是最自然的基础调式" << endl;
        cout << "• 内弦二指需按全音(F#)，是3(mi)音" << endl;
        cout << "• 指法最自然，适合初学者入门" << endl;
    }
    else if (key_signature == "G") {
        cout << "• G调(1=G): 空弦为5-2" << endl;
        cout << "• 内弦三指(G)是1(do)音" << endl;
        cout << "• 与D调关系：D调空弦音=G调的5音" << endl;
    }
    else if (key_signature == "F") {
        cout << "• F调(1=F): 空弦为6-3" << endl;
        cout << "• 特征指法：内弦一二指为半音关系(7-1)" << endl;
        cout << "• 注意内弦二指要按半音(F)" << endl;
    }
    else if (key_signature == "C") {
        cout << "• C调(1=C): 空弦为2-6" << endl;
        cout << "• 常用把位较高，第一把位内弦三指为1(do)" << endl;
    }
    else if (key_signature == "A") {
        cout << "• A调(1=A): 空弦为4-1" << endl;
        cout << "• 外弦空弦就是主音1，演奏方便" << endl;
    }
    else if (key_signature == "Bb") {
        cout << "• Bb调(1=Bb): 空弦为3-7" << endl;
        cout << "• 注意内弦一二指为半音关系" << endl;
    }
    else if (key_signature == "Eb") {
        cout << "• Eb调(1=Eb): 空弦为b7-4" << endl;
        cout << "• 降号调，注意音准控制" << endl;
    }
    else {
        cout << "• " << key_signature << "调: 请参考上方音位图练习" << endl;
        cout << "• 注意音准控制和变化音的指法" << endl;
    }

    // 通用提示
    cout << "══════════════════════════════════════════════" << endl;
    cout << "提示：上表中每个把位从上到下依次为：空弦、一指、二指、三指、四指" << endl;
    cout << "      '#'表示升半音，'b'表示降半音" << endl;
    cout << "      表中音高为理论值，实际演奏需根据听觉微调" << endl;
}

int main() {
    cout << "╔═══════════════════════════════════════╗" << endl;
    cout << "║       二胡调式音位计算器 v2.0        ║" << endl;
    cout << "║          （竖向显示版）               ║" << endl;
    cout << "╚═══════════════════════════════════════╝" << endl;
    cout << endl;
    cout << "【功能说明】" << endl;
    cout << "────────────" << endl;
    cout << "• 输入调式，自动生成二胡音位图" << endl;
    cout << "• 竖向显示四个把位的音高分布" << endl;
    cout << "• 简谱数字表示，清晰直观" << endl;
    cout << endl;
    cout << "【支持调式】" << endl;
    cout << "────────────" << endl;
    cout << "基础调：C, D, E, F, G, A, B" << endl;
    cout << "升号调：C#, D#, F#, G#, A#" << endl;
    cout << "降号调：Bb, Eb, Ab, Db, Gb" << endl;
    cout << endl;
    cout << "【输入示例】" << endl;
    cout << "────────────" << endl;
    cout << "D    → D大调 (1=D, 空弦1-5)" << endl;
    cout << "G    → G大调 (1=G, 空弦5-2)" << endl;
    cout << "F    → F大调 (1=F, 空弦6-3)" << endl;
    cout << "Bb   → 降B大调 (1=Bb)" << endl;
    cout << endl;
    cout << "══════════════════════════════════════════════" << endl;

    while (true) {
        string input;
        cout << "\n请输入调式 (输入'q'退出): ";
        cin >> input;

        if (input == "q" || input == "Q") {
            cout << "\n感谢使用！再见！" << endl;
            break;
        }

        // 验证输入格式
        bool valid = false;
        string processed_input;

        if (input.length() == 1) {
            char c = toupper(input[0]);
            if (c >= 'A' && c <= 'G') {
                valid = true;
                processed_input = string(1, c);
            }
        }
        else if (input.length() == 2) {
            char base = toupper(input[0]);
            char modifier = input[1];
            if (base >= 'A' && base <= 'G') {
                if (modifier == '#') {
                    valid = true;
                    processed_input = string(1, base) + "#";
                }
                else if (modifier == 'b' || modifier == 'B') {
                    valid = true;
                    processed_input = string(1, base) + "b";
                }
            }
        }

        if (!valid) {
            cout << "⚠ 无效的输入格式！请重新输入。" << endl;
            continue;
        }

        // 检查是否是有效音名
        bool is_valid_note = false;
        string check_note = process_key_signature(processed_input);
        for (const auto& note : chromatic_scale) {
            if (note == check_note) {
                is_valid_note = true;
                break;
            }
        }

        if (!is_valid_note) {
            cout << "⚠ 无效的音名！请使用标准音名" << endl;
            continue;
        }

        // 打印竖向音位图
        print_erhu_fingering_chart_vertical(processed_input);

        // 询问是否继续
        cout << "\n══════════════════════════════════════════════" << endl;
        cout << "是否继续计算其他调式？(y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'n' || choice == 'N') {
            cout << "\n感谢使用！再见！" << endl;
            break;
        }
        cout << "══════════════════════════════════════════════" << endl;
    }

    return 0;
}