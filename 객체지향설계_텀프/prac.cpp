#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include <vector>
using namespace std;

string arr[100];//�迭�� 20�� ����

int fp = 0;//������ 0������ 
int cp = 0;//����������
int np = 20;//���������� 20������ �ڷ� ����

ifstream readFile;

int page_num = 0; //���ο��� ������ �� üũ�ϴ� �뵵



class pageStore {//��ü �������� �迭�� ���� �����ϱ� ���� ������
public:
    vector<string> buf;

    int i = 0;//while�� ����
    string word;
    int word_size = 0;
    vector<string>::iterator it;

    int check = 0;//�ܾ�üũ
    string word_last;//������� ������ �ܾ� ����
    string last;//���� ������ �ܾ� ������ ��

    void store() {
        readFile.open("test.txt");//���� ������ 

        if (readFile.is_open()) {//���� ����
            while (!readFile.eof()) {//��������� �ݱ�
                if (check == 0) {
                    readFile >> word;//�ܾ�ֱ�
                    word_size += word.length() + 1;//������� �ѹ��� ���
                    buf.push_back(word);//word����ֱ�
                    buf.push_back(" ");//��������ֱ�
                }
                else if (check == 1) {//�������ܾ� �����ؾ���,
                    word_size += word_last.length() + 1;//������� �ѹ��� ���
                    buf.push_back(word_last);//word_last����ֱ�
                    buf.push_back(" ");//��������ֱ�

                    check = 0;//üũ�ٽ� 0���� �����
                }

                if (word_size > 75) {//������ 75���� Ŀ����
                    word_last = word;//������ �ܾ� ����
                    buf.pop_back();//������ ���� �����ϰ� 
                    buf.pop_back();
                    buf.push_back("\n");//����ĭ���� �ѱ��

                    for (it = buf.begin(); it != buf.end(); it++) {
                        arr[i].append(*it);//arr�� ����.
                    }
                    buf.clear();
                    word_size = 0;
                    i++;
                    check = 1;

                    if (i % 20 == 0) {//20�� �Ѿ�� ������ �϶����� 
                        page_num++;//������ �� ����
                    }
                    
                }
            }
            buf.push_back(word);//������ �� 75����Ʈ ���� ���� �� arr�� ��������.
            buf.push_back("\n");
            for (it = buf.begin(); it != buf.end()-2; it++) {
                arr[i].append(*it);
            }
            arr[i].push_back('\n');
            page_num++;
        }
            readFile.close();
    }
};


class nextPage { //����������
public:
    void cur_page() {   //���������� 
            cp = fp;
            while (fp <= cp && cp < np) {// 0 - 20
                if (cp < 9 ) {
                    cout << " " << cp + 1 << "| " << arr[cp];
                }
                else {
                    cout << cp + 1 << "| " << arr[cp];
                }
               
                cp++;
                if (cp == np) {//0 20 20
                    break;
                }
            }     
    }

    void next_page() {   //���� ������ �ε�  
        fp += 20;
        cp = fp;
        np += 20;

        int check = 0;

        //���� ���� �˻��� �ôµ� ���� �ٿ� ��� ������ ī��Ʈ ���� �� �ٸ�ŭ �ڷ� ������ ���� ���
        for (int i = fp; i < np; i++) {//���ۺ��� ������ �˻�
            if (arr[i].empty()) {//��� ����������
                break;
            }
            check++;
        }
        if (check < 20) {//�������� �ִ� ���

            np = fp + check;//������ ������ = ó�������� + check �� ������
            fp = np - 20;//ó�� ������ = ���� ������ - 20 
            cp = fp;//���� ������
        }
        while (fp <= cp && cp < np) {
            if (cp < 9)
                cout << " " << cp + 1 << "| " << arr[cp];
            else
                cout << cp + 1 << "| " << arr[cp];

            cp++;
            if (cp == np) {//���� �������� break;
                break;
            }
        }
    }
};

class previousPage {//����������
public:
    void pre_page() {//���� ������ �ε�޼ҵ�
        fp -= 20;
        cp = fp;
        np -= 20;

        int check = 0;
        if (fp < 0) {//���� fp�� 0���� �۾�����
            fp = 0;
            cp = 0;
            np = 20;
            check++;
        }

        while (fp <= cp && cp < np) {
            if (cp < 9) {
                cout << " " << cp + 1 << "| " << arr[cp];
            }
            else {
                cout << cp + 1 << "| " << arr[cp];
            }
            cp++;
            if (cp == np) {
                break;
            }
        }
    }
};

class insert { //����
public:
    void insert_word(string s) {
     
        string tmp = s.substr(2, s.length() - 3); //1,10,hello �����

        string str_arr[50];//�ڸ� �� ������ string �迭 
        char* str_buf = new char[30];//char�� ���� 
        strcpy(str_buf, tmp.c_str());

        int str_cnt = 0;
        char* tok = strtok(str_buf, ",");// , �������� �߶� �����ϱ�
        while (tok != nullptr) {
            str_arr[str_cnt++] = string(tok);
            tok = strtok(nullptr, ",");
        }

        int first = 0;//1�� ����ȯ
        std::stringstream sInt(str_arr[0]);
        sInt >> first;
        first--;
        first = first + fp;//���������� �Ѿ���� �װ� �ݿ��ؼ� �߰��� �� �ְ� ��.

        int second = 0;//2�� ����ȯ
        std::stringstream ssInt(str_arr[1]);
        ssInt >> second;

        string third = str_arr[2];//3���� �׳� ���� ���߷��� ����
                                
        int emp_cnt = 0;//���鼼��                                              
        for (int i = 0; i <= arr[first].length(); i++) {
            if ((arr[first])[i] == ' ') {//ù���� �ش� ĭ�� �����̶� ������
                emp_cnt++;//����ī��Ʈ �ø���
            }
            if (emp_cnt == second) {//���� ī��Ʈ�� �ι�° ���ڿ� ��������
                arr[first].insert(i," " + third);//�ش� �ε����� �ٿ� third ���� �߰�
                break;//�� �̷�� ���� ������
            }
        }
        
        int idx = first;//������ �ٺ��� üũ�ϸ� ��.
        string word;

        while (!(arr[idx].empty())) {//arr ����� ��� ����
            if (arr[idx].length() > 75) {//���� arr�迭���̰� 75���� Ŀ���� ���
                if (arr[idx].at(arr[idx].length()-2) == ' ') {//arr�ǵڰ� ' ' �� ��� ' ' �������ֱ�
                    arr[idx].replace(arr[idx].length()-2,arr[idx].length()-2,"");//�������
                }
                word = arr[idx].substr(arr[idx].rfind(' ')+1 , arr[idx].length()-1);//arr �� �ڿ��� ù��°�� ������ ����ã�Ƽ� �� �������� �ܾ� ��ȯ
                arr[idx].replace(arr[idx].rfind(' ')+1,arr[idx].length()-1, "");//���ٷ� ���� �ܾ�� ����
                arr[idx + 1].insert(0,word+" ");//���� �� 0��° ��ġ�� �̾Ƴ� �ܾ� ����
                arr[idx].append("\n");//�����ٷ� �ѱ��
            }
            else {//75�� ���� �ʴ´ٸ� ���� ������
                idx++;
            }
            
        }
    }
};

class del {//����
public:
    void delete_word(string s){
        string tmp = s.substr(2, s.length() - 3);

        string str_arr[50];//�ڸ� �� ������ string �迭 
        char* str_buf = new char[30];//char�� ���� 
        strcpy(str_buf, tmp.c_str());

        int str_cnt = 0;
        char* tok = strtok(str_buf, ",");// , �������� �߶� �����ϱ�
        while (tok != nullptr) {
            str_arr[str_cnt++] = string(tok);
            tok = strtok(nullptr, ",");
        }

        int first = 0;//1�� ����ȯ
        std::stringstream sInt(str_arr[0]);
        sInt >> first;
        first--;
        first = first + fp;

        int second = 0;//2�� ����ȯ
        std::stringstream ssInt(str_arr[1]);
        ssInt >> second;

        int emp_cnt = 0;//���鼼��
        int first_loc = 0;//ù���� ��ġ
        int second_loc = 0;//�ι�° ���� ��ġ

        for (int i = 0; i <= arr[first].length(); i++) {
            //�ܾ��� �հ� ���� ��ġ�� �˾Ƴ���
            if ((arr[first])[i] == ' ') {//arr[first]�� ��ġ�� ����� ������
                emp_cnt++;//���� cnt++
                if (emp_cnt == (second-1)) {//���� ������ ���ڰ� second������ -1�� ���ٸ�
                    first_loc = i;//���� ������ i ��ġ ����
                }
                if (emp_cnt == second) {
                    second_loc = i;//���� ������ i ��ġ ����
                    break;
                }
            }
        }
        if (first_loc == 0) {//�ش� ���� �� ���̸�
            arr[first].erase(first_loc, second_loc - first_loc+1);//���� �����ؼ� �ش� ��ġ ����
        }
        else {//�� �̿��� ��ġ�̸�
            arr[first].erase(first_loc, second_loc - first_loc);//���� ���ΰ� �ش� ��ġ ����
        }

        int idx = first;//������ �ٺ��� üũ�ϸ� ��.
        string word;
       
        while (!(arr[idx].empty())) {//����� ��� ����
            
            if (arr[idx].length() <= 75) {//�����ϰ� �� ���� �ش� ���� ���̰� 75�� �ȵ� ��� 

                int cnt = 0;//������ġã���
                for (int i = 0; i < arr[idx + 1].length(); i++) {//�������� ù ��������� ���� ���ϱ�
                    if ((arr[idx + 1])[i] == ' ') {//���ٰ� i��°�� ������
                        break;//ã������ ����������
                    }
                    cnt++;//��ġ ����
                }

                //�������� ù��° ���� �����ͼ� �����ٿ� �־��
                word = arr[idx + 1].substr(0, cnt);//0~ù��° ������̱��� ��ȯ
                arr[idx].insert(arr[idx].length()-1,word);//�����ٿ� ����
                
                //�����ͼ� �־��µ� 75�� ���� ���
                if (arr[idx].length() > 75) {
                    arr[idx].erase(arr[idx].rfind(' ') + 1,arr[idx].length()); //�����ߴ� ���ڿ� �����ϰ� 
                    arr[idx].append("\n");
                    idx++;//�����ٷ� �Ѿ��
                    continue;
                }
                //75�ȳ����� ���������� ���� �ǹǷ� 
                arr[idx + 1].erase(0, word.length() + 1);//������ ó���� �����
            }
            idx++;//�ε��� ����
        }
    }
};


class change { //����
public:
   void change_word(string s) {                      
       string tmp = s.substr(2, s.length()-3); //hello,bye �����
    
       string str_arr[50];//�ڸ� �� ������ string �迭 
       char* str_buf = new char[30];//char�� ����
       strcpy(str_buf, tmp.c_str());
       
       int str_cnt=0;
       char* tok = strtok(str_buf, ",");// , �������� �߶� �����ϱ�
       while (tok != nullptr) {
           str_arr[str_cnt++] = string(tok);
           tok = strtok(nullptr, ",");
       }

       int idx = 0;
       while (!arr[idx].empty()) {
           if (arr[idx].find(str_arr[0]) <75 ) {//�ش� ���ڿ� ������
               int in_num = arr[idx].find(str_arr[0]);//���� ��ġ �����ϰ�
               arr[idx].erase(arr[idx].find(str_arr[0]),str_arr[0].length());//�ش� ������ �����ϰ�
               arr[idx].insert(in_num,str_arr[1]);//���� ���� �ڸ��� ���ڿ� ����
           }
           idx++;//�ε��� ����
       }

        int rev = 0;//������ �� ù�ٺ��� üũ
        string word;

        while (!(arr[rev].empty())) {//arr ����� ��� ����
            if (arr[rev].length() >= 75) {//���� arr�迭���̰� 75���� Ŀ���� ���
              
                if (arr[rev].at(arr[rev].length()-2) == ' ') {//arr�ǵڰ� ' ' �� ��� ' ' �������ֱ�
                    arr[rev].replace(arr[rev].length()-2,arr[rev].length()-2,"");//�������
                }

                word = arr[rev].substr(arr[rev].rfind(' ')+1 , arr[rev].length()-1);//arr �� �ڿ��� ù��°�� ������ ����ã�Ƽ� �� �������� �ܾ� ��ȯ
                arr[rev].replace(arr[rev].rfind(' ')+1,arr[rev].length()-1, "");//���ٷ� ���� �ܾ�� ����
                arr[rev + 1].insert(0,word+" ");//���� �� 0��° ��ġ�� �̾Ƴ� �ܾ� ����
                arr[rev].append("\n");//�����ٷ� �ѱ��
            }
            else {//75�� �ȳ��� ��� �����ٷ� �Ѿ��
                rev++;
            }    
        }
    }
};

int isFind = 0;
class search {//ã��  
public:
	void search_word(string s) {
		int flag = 1;//�ű���� �׳� ���� ����
		int isContinue = 1;//�������� üũ
		int idx = 0;//�迭 �ε���
		int check = 2;//s(hello) �� �ܾ������ 2������
		int word_count = 0;

		while (flag == 1) {//0�̸� ����
			for (int i = 0; i <= arr[idx].size(); i++) {//�ٸ��� �ش� �ܾ� �ִ��� üũ�ϴ� �ݺ�����.
				
				if (word_count == 0 && (arr[idx])[i] == s.at(check)) {//ù�ܾ��̰� �ش� �ٿ��� h�ܾ� ������ 
					check++;//���� �ܾ�� �Ѿ�� e
					//�ε����� �״�� ����
					word_count++;//���ڼ� ������Ŵ.
					isContinue = 1;//�����̶�� üũ
					continue;//�Ʒ��� ����
				}

				if (isContinue == 1) {//�����϶�
					if ((arr[idx])[i] == s.at(check)) {//���� �ܾ ������ ��� ����
						isContinue = 1;//��Ƽ�� �״�� �α�
						word_count++;//�ܾ� �����ϱ� �� ����
						check++;//�����ܾ�� �Ѿ�� l
					}
					else {//������ �ƴϸ� �ٸ����� �ִ��� �ٽ� ã�ƺ���
						if (word_count == s.length() - 3) {//�ܾ� ã����
							flag = 0;//�ݺ��� ������
							continue;
						}
						isContinue = 0;//���� �ƴϴ� ǥ�ú�����
						check = 2;//üũ�� �ٽý���
						word_count = 0; //Ʋ�����ϱ� �ܾ� �� �ٽ� 0���� �����
					}
				}
			}
			//�ش��ٿ� ù�ܾ� ������ �ε��� �������Ѽ� ���� �ٷ� �ݺ��� ��� ��������
			idx++;
			if (arr[idx].empty()) {//�� ���Ҵµ��� ���ٸ�
				flag = 0;//�ݺ��� ������
			}
		}

		if (word_count == s.length() - 3) {//s(hello)�� �� 8�����ε� �ű⼭ s,(,) �� 3�� ����         //3���� ���Ŷ� ������ 
			//�ش� �� ���κ��� ����ϱ�
			fp = idx - 1;//ù������
			cp = idx - 1;//����������
			np = idx + 20 - 1;//����������
			isFind = 0;//�ܾ� ã�������

		}
		else {//��ã�� ���
			//�ƹ��͵� �� ã���� ���
			isFind = 1;
		}
	}
};

class save { //���Ͽ� ������ ����
public:
    ofstream writeFile;
    void close() {                                                                 
        //���� ���Ͽ� ������ arr���� ����  
        writeFile.open("test.txt");//���� ���� ���� ���ۼ�

        if (writeFile.is_open()) {
            int idx = 0;
            while (!arr[idx].empty()) {
                if (arr[idx].back() == '\n') {//�������� \n������
                    arr[idx].erase(arr[idx].find('\n'));//�����ֱ�
                }
                writeFile << arr[idx];//���Ͽ� ����
                idx++;
            }
        }
        writeFile.close();
    }
};

class consol {
public:
    int isCollect = 1;
public:

    //�Է¿��� ó�� �޼ҵ�
    string init_err(string s) {
        int not_cnt = 0;

        //���� �˻��
        int check = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s.at(i) == ' ') {
                check++;
                break;
            }
        }

        while (isCollect == 1) {                 //�Է¿��� ���� ��� ���������� ���� ������
         
            if (not_cnt > 0) {//�Է¿��� �� ������
                not_cnt = 0;
                nextPage().cur_page();//ù������ ���
                consol().re_con();//�ٽ��Է����ּ��� ���

                char ss[100] = { '0' };
                s = "";//�ٽ� �����
                cin.getline(ss, 100);   // �� �ٲ� ���ڱ��� �����Ͽ� 20�ڸ� �Է¹���

                for (int i = 0; i < sizeof(ss); i++) {
                    if (ss[i] == NULL) {//��� Ż��
                        break;
                    }
                    s += ss[i];
                }
                cout << "------------------------------------------------------------------------------" << endl;
            }
            
            //idsc�ε� ��ȣ�� ���� ���
            if (((s.at(0) == 'i' || s.at(0) == 'd' || s.at(0) == 's' || s.at(0) == 'c') && (s.at(1) != '(' || s.back() != ')'))) {
                not_cnt++;
            }
            // ���� ���� ���ڰ� idsc tnp �̿��� ��� 
            else if (s.at(0) != 'i' && s.at(0) != 'd' && s.at(0) != 's' && s.at(0) != 'c' && s.at(0) != 't' && s.at(0) != 'n' && s.at(0) != 'p') {
                not_cnt++;
            }
            //���� ���� ���
            else if (check != 0) {
                not_cnt++;
            }
            //i�Է� ���� ó��
            else if (s.at(0) == 'i') {
                int cnt = 0;
                for (int i = 0; i < s.length(); i++) {
                    if (s.at(i) == ',') {//��ǥ ���� ����
                        cnt++;
                    }
                }
                // , �� 2���� �ƴ� ��� h(1,1)
                if (cnt != 2) {
                    not_cnt++;
                }
                
                // ���ڿ� ��ȣ �� ���  , �� ���� �̿��� ��� i(%,%,hello)
                string a = s.substr(2,s.find(',')-2);//ù����
                string b = s.substr(s.find(',') + 1, s.rfind(',') - s.find(',')-1);//�ι�° ����
                int no_digit = 0;
                for (char const& c : a) {
                    if (isdigit(c) == 0) {//���ھƴϸ�
                        no_digit++;
                    }
                    //���ڸ� �׳� �Ѿ
                }
                for (char const& d : b) {
                    if (isdigit(d) == 0) {//���ھƴϸ�
                        no_digit++;
                    }
                    //���ڸ� �׳� �Ѿ
                }
                cout << no_digit << endl;
                if (no_digit != 0) {
                    not_cnt++;
                    continue;
                }

                //hello�� 75����Ʈ �̻� ���� �� x (1,1,heolllllllllllllllllllllllllllllllllllllllllll)
                string find = s.substr(s.rfind(',')+1,s.length()-s.rfind(','));
         
                if (find.length() > 75) {
                    not_cnt++;
                    continue;
                }

                //�� 20�� �̻� x (22,1,heool)
                int i = 0;
                std::stringstream ssInt(s.substr(2, s.find(',')));
                ssInt >> i;
                if (i > 20) {
                    not_cnt++;
                    continue;
                }

                // �ش� ���� �ܾ� ���� ��� i(1,80,hello)
                int first = 0;
                std::stringstream firstInt(s.substr(2,s.find(',')-2));//2������ ,������ �ڸ��� =  �� ��ȣ
                firstInt >> first;

                int second = 0;
                std::stringstream secondInt(s.substr(s.find(',')+1,s.rfind(',')-s.find(',')));//��ȣ����
                secondInt >> second;

                int word_cnt = 0;
                for (int i = 0; i <= arr[fp+first-1].length(); i++) {
                    if ((arr[fp+first-1])[i] == ' ') {//�ٲٷ��� ������ + �� ��ȣ
                        word_cnt++;
                    }
                }
                
                if (second > word_cnt) {
                    not_cnt++;
                    continue;
                }
                else {
                    return s;
                }
            }
            else if (s.at(0) == 'd') {
                int cnt = 0;
                for (int i = 0; i < s.length(); i++) {
                    if (s.at(i) == ',') {//��ǥ ���� ����
                        cnt++;
                    }
                }
                //,�� 1���� �ƴ� ��� d(1)
                if (cnt != 1) {
                    not_cnt++;
                }
                // ���ڿ� ��ȣ �� ��� d(%,%)
                string a = s.substr(2, s.find(',') - 2);//ù����
                string b = s.substr(s.rfind(',')+1,s.length()-s.rfind(',')-2);//�ι�° ����

                int no_digit = 0;
                for (char const& c : a) {
                    if (isdigit(c) == 0) {//���ھƴϸ�
                        no_digit++;
                    }
                    //���ڸ� �׳� �Ѿ
                }
                for (char const& c : b) {
                    if (isdigit(c) == 0) {//���ھƴϸ�
                        no_digit++;
                    }
                    //���ڸ� �׳� �Ѿ
                }

                if (no_digit != 0) {
                    not_cnt++;
                    continue;
                }


                // 20�� �̻� ���� x d(22,1)
                int i = 0;
                std::stringstream ssInt(s.substr(2, s.find(',')));
                ssInt >> i;
                //cout << "ssint : " << i << endl;
                if (i > 20) {
                    not_cnt++;
                    continue;
                }

                // �ܾ� ���� ���� ��� x d(2,22)
               int first = 0;
                std::stringstream firstInt(s.substr(2, s.find(',') - 2));//2������ ,������ �ڸ��� =  �� ��ȣ
                firstInt >> first;
                
                int second = 0;
                std::stringstream secondInt(s.substr(s.find(',') + 1, s.length()-1-s.find(',')));//��ȣ����
                secondInt >> second;

                int word_cnt = 0;
                for (int i = 0; i <= arr[fp + first - 1].length(); i++) {
                    if ((arr[fp + first - 1])[i] == ' ') {//�ٲٷ��� ������ + �� ��ȣ
                        word_cnt++;
                    }
                }

                if (second > word_cnt) {
                    not_cnt++;
                    continue;
                }
                else {
                    return s;
                }
            }
            else if (s.at(0) == 's') {                                                              
                
                int cnt = 0;
                for (int i = 0; i < s.length(); i++) {
                    if (s.at(i) == ',') {//��ǥ ���� ����
                        cnt++;
                    }
                }
                //,���� ��� x
                if (cnt != 0) {
                    not_cnt++;
                    continue;
                }
                else {
                    return s;
                }
            }
            else if (s.at(0) == 'c') {
                int cnt = 0;
                for (int i = 0; i < s.length(); i++) {
                    if (s.at(i) == ',') {//��ǥ ���� ����
                        cnt++;
                    }
                }
                //, �� 1���� �ƴ� ��� c(change) x
                if (cnt != 1) {
                    not_cnt++;
                    continue;
                }
                //�ٲٷ��� �ܾ�� 75����Ʈ ���� ��� x c(change,gk1111111111111111111)
                string find = s.substr(s.rfind(',')+1, s.length() - 1);
                if (find.length() > 75) {
                    not_cnt++;
                    continue;
                }
                else {
                    return s;
                }

            }
            else if (s.at(0) == 't' || s.at(0) == 'n' || s.at(0) == 'p') {
                //�ڿ� �� �� ����� ��� x 
                if (s.length() != 1) {
                    not_cnt++;
                    continue;
                }
                else {
                    return s;
                }
            }
            else {//����� ������ ���
                isCollect = 0;
                return s;
            }
        }
    }

    //�⺻ con
    void con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(�ָܼ޽���) " << endl;//�ָܼ޽��� ��µǰ� �ϱ�
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "�Է� : ";
    }

    //���Է�con
    void re_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(�ָܼ޽���) �߸��Է��߽��ϴ�. �ٽ� �Է��� �ּ���" << endl;//�ָܼ޽��� ��µǰ� �ϱ�
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "�Է� : ";
    }
    //ù������ ��� con
    void first_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(�ָܼ޽���) ù ��° �������Դϴ�." << endl;//�ָܼ޽��� ��µǰ� �ϱ�
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "�Է� : ";
    }
    //������ ������ ��� con
    void last_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(�ָܼ޽���) ������ �������Դϴ�." << endl;//�ָܼ޽��� ��µǰ� �ϱ�
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "�Է� : ";
    }
    //�ܾ���� ��� con
    void noWord_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(�ָܼ޽���) �ش� �ܾ �����ϴ�." << endl;//�ָܼ޽��� ��µǰ� �ϱ�
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "�Է� : ";
    }
};





int main() {
    pageStore().store();//�ϴ� ����
    nextPage().cur_page();//���� ������ ���.

    string s;
    //�ʱ� ���
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "(�ָܼ޽���)" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "�Է� : ";  //���⿡ �Է� ���۵ǰ��ϱ�.

    //�Էµ����� �ſ� ���� action ���ϱ�
    char ss[100] = {'0'};
    cin.getline(ss, 100);   // �� �ٲ� ���ڱ��� �����Ͽ� 20�ڸ� �Է¹���
    
    for (int i = 0; i < sizeof(ss); i++) {
        if (ss[i] == NULL) {//��� Ż��
            break;
        }
        s += ss[i];
    }

    int cnt = 0;
    cout << "------------------------------------------------------------------------------" << endl;
  
    //�Է¿���ó���ϴ� Ŭ���� �޼ҵ�
    s = consol().init_err(s);

    int n_cnt = 0;
    int p_cnt = 0;
    while (1) {//�Է��� t�� ���ö� �����ϰ� ����

        if (cnt >= 1) {//������ �ѹ� �Է� ���� �� ���� �Է¹ް� �ϱ� ���ؼ� ����.
            char ss[100] = { '0' };
            s = "";//�ٽ� �����
            cin.getline(ss, 100);   // �� �ٲ� ���ڱ��� �����Ͽ� 20�ڸ� �Է¹���

            for (int i = 0; i < sizeof(ss); i++) {
                if (ss[i] == NULL) {//��� Ż��
                    break;
                }
                s += ss[i];
            }
            cout << "------------------------------------------------------------------------------" << endl;
            //�Է¿���ó���ϴ� Ŭ���� �޼ҵ�
             s = consol().init_err(s);                                                                
        }

        if (s == "n") {//����������                                        
            n_cnt++;
            if (n_cnt+p_cnt >= page_num) {//���������������� n�� �����ٸ�      
                n_cnt--;
                nextPage().cur_page();//���������� ���
                consol().last_con();//�����޽��� ���
            }
            else {
                nextPage().next_page();//���������� ����ϰ�
                consol().con();
            }
            
        }
        else if (s == "p") {//����������                                   
            p_cnt--;
            if (fp - 20 < 0 ) {
                previousPage().pre_page();
                consol().first_con();
            }
            if (p_cnt+n_cnt < 0) {//���� �������� ���� �������� ���� ���� ���� - ���             
                p_cnt++;
            }
            else {
                previousPage().pre_page();
                consol().con();
            }
        }
        else if (s.front() == 'i') {//����                        
            insert().insert_word(s);
            nextPage().cur_page();
            consol().con();
        }
        else if (s.front() == 'd') {//����
            del().delete_word(s);
            nextPage().cur_page();
            consol().con();
        }
        else if (s.front() == 'c') {//����                           
            change().change_word(s);//�����ϰ�                                    
            nextPage().cur_page();//�ٽ� ���� �ִ� ������ ����ϰ�
            consol().con();//�ܼ�â ���

        }
        else if (s.front() == 's') {//ã��       
            search().search_word(s);
            if (isFind == 0) {//�ܾ� �������
                nextPage().cur_page();
                consol().con();//�������
            }
            else {//�ܾ� �������
                nextPage().cur_page();
                consol().noWord_con();//�ٽ� ���
            }
        }
        else if (s.front() == 't') {//���� �� ����       
            save().close();
            return 0;
        }
        cnt++;
    }
    return 0;
}