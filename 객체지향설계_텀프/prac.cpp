#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include <vector>
using namespace std;

string arr[100];//배열에 20줄 저장

int fp = 0;//시작은 0페이지 
int cp = 0;//현재페이지
int np = 20;//끝페이지는 20페이지 뒤로 시작

ifstream readFile;

int page_num = 0; //메인에서 페이지 수 체크하는 용도



class pageStore {//전체 페이지를 배열에 먼저 저장하기 위해 만들음
public:
    vector<string> buf;

    int i = 0;//while문 인자
    string word;
    int word_size = 0;
    vector<string>::iterator it;

    int check = 0;//단어체크
    string word_last;//사라지는 마지막 단어 저장
    string last;//가장 마지막 단어 저장할 곳

    void store() {
        readFile.open("test.txt");//파일 가져옴 

        if (readFile.is_open()) {//파일 열기
            while (!readFile.eof()) {//내용없으면 닫기
                if (check == 0) {
                    readFile >> word;//단어넣기
                    word_size += word.length() + 1;//띄어쓰기까지 한번에 계산
                    buf.push_back(word);//word집어넣기
                    buf.push_back(" ");//띄어쓰기집어넣기
                }
                else if (check == 1) {//마지막단어 저장해야함,
                    word_size += word_last.length() + 1;//띄어쓰기까지 한번에 계산
                    buf.push_back(word_last);//word_last집어넣기
                    buf.push_back(" ");//띄어쓰기집어넣기

                    check = 0;//체크다시 0으로 만들기
                }

                if (word_size > 75) {//사이즈 75보다 커지면
                    word_last = word;//마지막 단어 저장
                    buf.pop_back();//마지막 원소 삭제하고 
                    buf.pop_back();
                    buf.push_back("\n");//다음칸으로 넘기기

                    for (it = buf.begin(); it != buf.end(); it++) {
                        arr[i].append(*it);//arr에 저장.
                    }
                    buf.clear();
                    word_size = 0;
                    i++;
                    check = 1;

                    if (i % 20 == 0) {//20줄 넘어가는 페이지 일때마다 
                        page_num++;//페이지 수 저장
                    }
                    
                }
            }
            buf.push_back(word);//마지막 줄 75바이트 보다 작을 때 arr에 저장해줌.
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


class nextPage { //다음페이지
public:
    void cur_page() {   //현재페이지 
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

    void next_page() {   //다음 페이지 로드  
        fp += 20;
        cp = fp;
        np += 20;

        int check = 0;

        //만약 먼저 검색해 봤는데 다음 줄에 빈게 있으면 카운트 세고 그 줄만큼 뒤로 돌려서 같이 출력
        for (int i = fp; i < np; i++) {//시작부터 끝까지 검사
            if (arr[i].empty()) {//비면 빠져나오기
                break;
            }
            check++;
        }
        if (check < 20) {//빈페이지 있는 경우

            np = fp + check;//마지막 페이지 = 처음페이지 + check 할 페이지
            fp = np - 20;//처음 페이지 = 나중 페이지 - 20 
            cp = fp;//현재 페이지
        }
        while (fp <= cp && cp < np) {
            if (cp < 9)
                cout << " " << cp + 1 << "| " << arr[cp];
            else
                cout << cp + 1 << "| " << arr[cp];

            cp++;
            if (cp == np) {//끝에 같아지면 break;
                break;
            }
        }
    }
};

class previousPage {//이전페이지
public:
    void pre_page() {//이전 페이지 로드메소드
        fp -= 20;
        cp = fp;
        np -= 20;

        int check = 0;
        if (fp < 0) {//만약 fp가 0보다 작아지면
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

class insert { //삽입
public:
    void insert_word(string s) {
     
        string tmp = s.substr(2, s.length() - 3); //1,10,hello 저장됨

        string str_arr[50];//자른 애 보관할 string 배열 
        char* str_buf = new char[30];//char로 변경 
        strcpy(str_buf, tmp.c_str());

        int str_cnt = 0;
        char* tok = strtok(str_buf, ",");// , 기준으로 잘라서 저장하기
        while (tok != nullptr) {
            str_arr[str_cnt++] = string(tok);
            tok = strtok(nullptr, ",");
        }

        int first = 0;//1번 형변환
        std::stringstream sInt(str_arr[0]);
        sInt >> first;
        first--;
        first = first + fp;//다음페이지 넘어갔을때 그거 반영해서 추가할 수 있게 함.

        int second = 0;//2번 형변환
        std::stringstream ssInt(str_arr[1]);
        ssInt >> second;

        string third = str_arr[2];//3번은 그냥 순서 맞추려고 저장
                                
        int emp_cnt = 0;//공백세기                                              
        for (int i = 0; i <= arr[first].length(); i++) {
            if ((arr[first])[i] == ' ') {//첫줄의 해당 칸이 공백이랑 같으면
                emp_cnt++;//공백카운트 늘리기
            }
            if (emp_cnt == second) {//공백 카운트가 두번째 숫자와 같아지면
                arr[first].insert(i," " + third);//해당 인덱스의 줄에 third 내용 추가
                break;//줄 미루고 빠져 나오기
            }
        }
        
        int idx = first;//수정한 줄부터 체크하면 됌.
        string word;

        while (!(arr[idx].empty())) {//arr 비었을 경우 종료
            if (arr[idx].length() > 75) {//만약 arr배열길이가 75보다 커졌을 경우
                if (arr[idx].at(arr[idx].length()-2) == ' ') {//arr맨뒤가 ' ' 일 경우 ' ' 삭제해주기
                    arr[idx].replace(arr[idx].length()-2,arr[idx].length()-2,"");//공백삭제
                }
                word = arr[idx].substr(arr[idx].rfind(' ')+1 , arr[idx].length()-1);//arr 맨 뒤에서 첫번째로 나오는 공백찾아서 맨 끝까지의 단어 반환
                arr[idx].replace(arr[idx].rfind(' ')+1,arr[idx].length()-1, "");//담줄로 보낸 단어는 삭제
                arr[idx + 1].insert(0,word+" ");//다음 줄 0번째 위치에 뽑아낸 단어 저장
                arr[idx].append("\n");//다음줄로 넘기기
            }
            else {//75가 넘지 않는다면 빠져 나오기
                idx++;
            }
            
        }
    }
};

class del {//삭제
public:
    void delete_word(string s){
        string tmp = s.substr(2, s.length() - 3);

        string str_arr[50];//자른 애 보관할 string 배열 
        char* str_buf = new char[30];//char로 변경 
        strcpy(str_buf, tmp.c_str());

        int str_cnt = 0;
        char* tok = strtok(str_buf, ",");// , 기준으로 잘라서 저장하기
        while (tok != nullptr) {
            str_arr[str_cnt++] = string(tok);
            tok = strtok(nullptr, ",");
        }

        int first = 0;//1번 형변환
        std::stringstream sInt(str_arr[0]);
        sInt >> first;
        first--;
        first = first + fp;

        int second = 0;//2번 형변환
        std::stringstream ssInt(str_arr[1]);
        ssInt >> second;

        int emp_cnt = 0;//공백세기
        int first_loc = 0;//첫공백 위치
        int second_loc = 0;//두번째 공백 위치

        for (int i = 0; i <= arr[first].length(); i++) {
            //단어의 앞과 뒤의 위치를 알아내기
            if ((arr[first])[i] == ' ') {//arr[first]의 위치가 공백과 같으면
                emp_cnt++;//공백 cnt++
                if (emp_cnt == (second-1)) {//만약 공백의 숫자가 second숫자의 -1과 같다면
                    first_loc = i;//시작 공백의 i 위치 저장
                }
                if (emp_cnt == second) {
                    second_loc = i;//끝날 공백의 i 위치 저장
                    break;
                }
            }
        }
        if (first_loc == 0) {//해당 줄의 맨 앞이면
            arr[first].erase(first_loc, second_loc - first_loc+1);//띄어쓰기 포함해서 해당 위치 삭제
        }
        else {//그 이외의 위치이면
            arr[first].erase(first_loc, second_loc - first_loc);//띄어쓰기 놔두고 해당 위치 삭제
        }

        int idx = first;//수정한 줄부터 체크하면 됌.
        string word;
       
        while (!(arr[idx].empty())) {//비었을 경우 종료
            
            if (arr[idx].length() <= 75) {//삭제하고 난 뒤의 해당 줄의 길이가 75가 안될 경우 

                int cnt = 0;//공백위치찾기용
                for (int i = 0; i < arr[idx + 1].length(); i++) {//다음줄의 첫 공백까지의 길이 구하기
                    if ((arr[idx + 1])[i] == ' ') {//돌다가 i번째랑 같으면
                        break;//찾았으면 빠져나오기
                    }
                    cnt++;//위치 증가
                }

                //다음줄의 첫번째 워드 가져와서 현재줄에 넣어보기
                word = arr[idx + 1].substr(0, cnt);//0~첫번째 공백길이까지 반환
                arr[idx].insert(arr[idx].length()-1,word);//원래줄에 삽입
                
                //가져와서 넣었는데 75가 넘을 경우
                if (arr[idx].length() > 75) {
                    arr[idx].erase(arr[idx].rfind(' ') + 1,arr[idx].length()); //삽입했던 문자열 삭제하고 
                    arr[idx].append("\n");
                    idx++;//다음줄로 넘어가기
                    continue;
                }
                //75안넘으면 정상적으로 가면 되므로 
                arr[idx + 1].erase(0, word.length() + 1);//다음줄 처음꺼 지우기
            }
            idx++;//인덱스 증가
        }
    }
};


class change { //변경
public:
   void change_word(string s) {                      
       string tmp = s.substr(2, s.length()-3); //hello,bye 저장됨
    
       string str_arr[50];//자른 애 보관할 string 배열 
       char* str_buf = new char[30];//char로 변경
       strcpy(str_buf, tmp.c_str());
       
       int str_cnt=0;
       char* tok = strtok(str_buf, ",");// , 기준으로 잘라서 저장하기
       while (tok != nullptr) {
           str_arr[str_cnt++] = string(tok);
           tok = strtok(nullptr, ",");
       }

       int idx = 0;
       while (!arr[idx].empty()) {
           if (arr[idx].find(str_arr[0]) <75 ) {//해당 문자열 있으면
               int in_num = arr[idx].find(str_arr[0]);//넣을 위치 저장하고
               arr[idx].erase(arr[idx].find(str_arr[0]),str_arr[0].length());//해당 범위값 삭제하고
               arr[idx].insert(in_num,str_arr[1]);//원래 글자 자리에 문자열 삽입
           }
           idx++;//인덱스 증가
       }

        int rev = 0;//수정된 후 첫줄부터 체크
        string word;

        while (!(arr[rev].empty())) {//arr 비었을 경우 종료
            if (arr[rev].length() >= 75) {//만약 arr배열길이가 75보다 커졌을 경우
              
                if (arr[rev].at(arr[rev].length()-2) == ' ') {//arr맨뒤가 ' ' 일 경우 ' ' 삭제해주기
                    arr[rev].replace(arr[rev].length()-2,arr[rev].length()-2,"");//공백삭제
                }

                word = arr[rev].substr(arr[rev].rfind(' ')+1 , arr[rev].length()-1);//arr 맨 뒤에서 첫번째로 나오는 공백찾아서 맨 끝까지의 단어 반환
                arr[rev].replace(arr[rev].rfind(' ')+1,arr[rev].length()-1, "");//담줄로 보낸 단어는 삭제
                arr[rev + 1].insert(0,word+" ");//다음 줄 0번째 위치에 뽑아낸 단어 저장
                arr[rev].append("\n");//다음줄로 넘기기
            }
            else {//75가 안넘을 경우 다음줄로 넘어가기
                rev++;
            }    
        }
    }
};

int isFind = 0;
class search {//찾기  
public:
	void search_word(string s) {
		int flag = 1;//거기까지 그냥 강제 종료
		int isContinue = 1;//연속인지 체크
		int idx = 0;//배열 인덱스
		int check = 2;//s(hello) 면 단어시작은 2부터임
		int word_count = 0;

		while (flag == 1) {//0이면 종료
			for (int i = 0; i <= arr[idx].size(); i++) {//줄마다 해당 단어 있는지 체크하는 반복문임.
				
				if (word_count == 0 && (arr[idx])[i] == s.at(check)) {//첫단어이고 해당 줄에서 h단어 맞으면 
					check++;//다음 단어로 넘어가고 e
					//인덱스는 그대로 놓고
					word_count++;//글자수 증가시킴.
					isContinue = 1;//연속이라고 체크
					continue;//아래는 무시
				}

				if (isContinue == 1) {//연속일때
					if ((arr[idx])[i] == s.at(check)) {//다음 단어도 같으면 계속 연속
						isContinue = 1;//컨티뉴 그대로 두기
						word_count++;//단어 같으니까 수 증가
						check++;//다음단어로 넘어가기 l
					}
					else {//연속이 아니면 다른곳에 있는지 다시 찾아보기
						if (word_count == s.length() - 3) {//단어 찾으면
							flag = 0;//반복문 나가기
							continue;
						}
						isContinue = 0;//연속 아니다 표시보내기
						check = 2;//체크도 다시시작
						word_count = 0; //틀렸으니까 단어 수 다시 0으로 만들기
					}
				}
			}
			//해당줄에 첫단어 없으면 인덱스 증가시켜서 다음 줄로 반복문 계속 돌려보기
			idx++;
			if (arr[idx].empty()) {//다 돌았는데도 없다면
				flag = 0;//반복문 나가기
			}
		}

		if (word_count == s.length() - 3) {//s(hello)면 총 8글자인데 거기서 s,(,) 의 3개 빼기         //3글자 뺀거랑 같으면 
			//해당 줄 라인부터 출력하기
			fp = idx - 1;//첫페이지
			cp = idx - 1;//현재페이지
			np = idx + 20 - 1;//다음페이지
			isFind = 0;//단어 찾았을경우

		}
		else {//못찾을 경우
			//아무것도 못 찾았을 경우
			isFind = 1;
		}
	}
};

class save { //파일에 저장후 종료
public:
    ofstream writeFile;
    void close() {                                                                 
        //같은 파일에 수정된 arr내용 저장  
        writeFile.open("test.txt");//파일 쓰기 모드로 재작성

        if (writeFile.is_open()) {
            int idx = 0;
            while (!arr[idx].empty()) {
                if (arr[idx].back() == '\n') {//마지막에 \n있으면
                    arr[idx].erase(arr[idx].find('\n'));//지워주기
                }
                writeFile << arr[idx];//파일에 저장
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

    //입력오류 처리 메소드
    string init_err(string s) {
        int not_cnt = 0;

        //공백 검사용
        int check = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s.at(i) == ' ') {
                check++;
                break;
            }
        }

        while (isCollect == 1) {                 //입력오류 생길 경우 맞을때까지 루프 못나가
         
            if (not_cnt > 0) {//입력오류 또 만들경우
                not_cnt = 0;
                nextPage().cur_page();//첫페이지 출력
                consol().re_con();//다시입력해주세요 출력

                char ss[100] = { '0' };
                s = "";//다시 비워줌
                cin.getline(ss, 100);   // 줄 바꿈 문자까지 포함하여 20자를 입력받음

                for (int i = 0; i < sizeof(ss); i++) {
                    if (ss[i] == NULL) {//비면 탈출
                        break;
                    }
                    s += ss[i];
                }
                cout << "------------------------------------------------------------------------------" << endl;
            }
            
            //idsc인데 괄호가 없을 경우
            if (((s.at(0) == 'i' || s.at(0) == 'd' || s.at(0) == 's' || s.at(0) == 'c') && (s.at(1) != '(' || s.back() != ')'))) {
                not_cnt++;
            }
            // 가장 앞의 문자가 idsc tnp 이외일 경우 
            else if (s.at(0) != 'i' && s.at(0) != 'd' && s.at(0) != 's' && s.at(0) != 'c' && s.at(0) != 't' && s.at(0) != 'n' && s.at(0) != 'p') {
                not_cnt++;
            }
            //공백 들어올 경우
            else if (check != 0) {
                not_cnt++;
            }
            //i입력 오류 처리
            else if (s.at(0) == 'i') {
                int cnt = 0;
                for (int i = 0; i < s.length(); i++) {
                    if (s.at(i) == ',') {//쉼표 개수 세기
                        cnt++;
                    }
                }
                // , 가 2개가 아닐 경우 h(1,1)
                if (cnt != 2) {
                    not_cnt++;
                }
                
                // 숫자에 기호 들어갈 경우  , 즉 숫자 이외일 경우 i(%,%,hello)
                string a = s.substr(2,s.find(',')-2);//첫문자
                string b = s.substr(s.find(',') + 1, s.rfind(',') - s.find(',')-1);//두번째 문자
                int no_digit = 0;
                for (char const& c : a) {
                    if (isdigit(c) == 0) {//숫자아니면
                        no_digit++;
                    }
                    //숫자면 그냥 넘어가
                }
                for (char const& d : b) {
                    if (isdigit(d) == 0) {//숫자아니면
                        no_digit++;
                    }
                    //숫자면 그냥 넘어가
                }
                cout << no_digit << endl;
                if (no_digit != 0) {
                    not_cnt++;
                    continue;
                }

                //hello에 75바이트 이상 넣을 수 x (1,1,heolllllllllllllllllllllllllllllllllllllllllll)
                string find = s.substr(s.rfind(',')+1,s.length()-s.rfind(','));
         
                if (find.length() > 75) {
                    not_cnt++;
                    continue;
                }

                //줄 20개 이상 x (22,1,heool)
                int i = 0;
                std::stringstream ssInt(s.substr(2, s.find(',')));
                ssInt >> i;
                if (i > 20) {
                    not_cnt++;
                    continue;
                }

                // 해당 줄의 단어 없을 경우 i(1,80,hello)
                int first = 0;
                std::stringstream firstInt(s.substr(2,s.find(',')-2));//2번부터 ,전까지 자르기 =  줄 번호
                firstInt >> first;

                int second = 0;
                std::stringstream secondInt(s.substr(s.find(',')+1,s.rfind(',')-s.find(',')));//번호개수
                secondInt >> second;

                int word_cnt = 0;
                for (int i = 0; i <= arr[fp+first-1].length(); i++) {
                    if ((arr[fp+first-1])[i] == ' ') {//바꾸려는 페이지 + 줄 번호
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
                    if (s.at(i) == ',') {//쉼표 개수 세기
                        cnt++;
                    }
                }
                //,가 1개가 아닐 경우 d(1)
                if (cnt != 1) {
                    not_cnt++;
                }
                // 숫자에 기호 들어갈 경우 d(%,%)
                string a = s.substr(2, s.find(',') - 2);//첫문자
                string b = s.substr(s.rfind(',')+1,s.length()-s.rfind(',')-2);//두번째 문자

                int no_digit = 0;
                for (char const& c : a) {
                    if (isdigit(c) == 0) {//숫자아니면
                        no_digit++;
                    }
                    //숫자면 그냥 넘어가
                }
                for (char const& c : b) {
                    if (isdigit(c) == 0) {//숫자아니면
                        no_digit++;
                    }
                    //숫자면 그냥 넘어가
                }

                if (no_digit != 0) {
                    not_cnt++;
                    continue;
                }


                // 20줄 이상 선택 x d(22,1)
                int i = 0;
                std::stringstream ssInt(s.substr(2, s.find(',')));
                ssInt >> i;
                //cout << "ssint : " << i << endl;
                if (i > 20) {
                    not_cnt++;
                    continue;
                }

                // 단어 개수 넘을 경우 x d(2,22)
               int first = 0;
                std::stringstream firstInt(s.substr(2, s.find(',') - 2));//2번부터 ,전까지 자르기 =  줄 번호
                firstInt >> first;
                
                int second = 0;
                std::stringstream secondInt(s.substr(s.find(',') + 1, s.length()-1-s.find(',')));//번호개수
                secondInt >> second;

                int word_cnt = 0;
                for (int i = 0; i <= arr[fp + first - 1].length(); i++) {
                    if ((arr[fp + first - 1])[i] == ' ') {//바꾸려는 페이지 + 줄 번호
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
                    if (s.at(i) == ',') {//쉼표 개수 세기
                        cnt++;
                    }
                }
                //,있을 경우 x
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
                    if (s.at(i) == ',') {//쉼표 개수 세기
                        cnt++;
                    }
                }
                //, 가 1개가 아닐 경우 c(change) x
                if (cnt != 1) {
                    not_cnt++;
                    continue;
                }
                //바꾸려는 단어개수 75바이트 넘을 경우 x c(change,gk1111111111111111111)
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
                //뒤에 뭐 더 따라올 경우 x 
                if (s.length() != 1) {
                    not_cnt++;
                    continue;
                }
                else {
                    return s;
                }
            }
            else {//제대로 들어왔을 경우
                isCollect = 0;
                return s;
            }
        }
    }

    //기본 con
    void con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(콘솔메시지) " << endl;//콘솔메시지 출력되게 하기
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "입력 : ";
    }

    //재입력con
    void re_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(콘솔메시지) 잘못입력했습니다. 다시 입력해 주세요" << endl;//콘솔메시지 출력되게 하기
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "입력 : ";
    }
    //첫페이지 출력 con
    void first_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(콘솔메시지) 첫 번째 페이지입니다." << endl;//콘솔메시지 출력되게 하기
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "입력 : ";
    }
    //마지막 페이지 출력 con
    void last_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(콘솔메시지) 마지막 페이지입니다." << endl;//콘솔메시지 출력되게 하기
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "입력 : ";
    }
    //단어없음 출력 con
    void noWord_con() {
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "(콘솔메시지) 해당 단어가 없습니다." << endl;//콘솔메시지 출력되게 하기
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "입력 : ";
    }
};





int main() {
    pageStore().store();//일단 저장
    nextPage().cur_page();//현재 페이지 출력.

    string s;
    //초기 출력
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "(콘솔메시지)" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "입력 : ";  //여기에 입력 시작되게하기.

    //입력들어오는 거에 따라 action 취하기
    char ss[100] = {'0'};
    cin.getline(ss, 100);   // 줄 바꿈 문자까지 포함하여 20자를 입력받음
    
    for (int i = 0; i < sizeof(ss); i++) {
        if (ss[i] == NULL) {//비면 탈출
            break;
        }
        s += ss[i];
    }

    int cnt = 0;
    cout << "------------------------------------------------------------------------------" << endl;
  
    //입력오류처리하는 클래스 메소드
    s = consol().init_err(s);

    int n_cnt = 0;
    int p_cnt = 0;
    while (1) {//입력이 t가 들어올때 저장하고 종료

        if (cnt >= 1) {//위에서 한번 입력 받은 후 부터 입력받게 하기 위해서 만듦.
            char ss[100] = { '0' };
            s = "";//다시 비워줌
            cin.getline(ss, 100);   // 줄 바꿈 문자까지 포함하여 20자를 입력받음

            for (int i = 0; i < sizeof(ss); i++) {
                if (ss[i] == NULL) {//비면 탈출
                    break;
                }
                s += ss[i];
            }
            cout << "------------------------------------------------------------------------------" << endl;
            //입력오류처리하는 클래스 메소드
             s = consol().init_err(s);                                                                
        }

        if (s == "n") {//다음페이지                                        
            n_cnt++;
            if (n_cnt+p_cnt >= page_num) {//마지막페이지에서 n을 누른다면      
                n_cnt--;
                nextPage().cur_page();//현재페이지 출력
                consol().last_con();//에러메시지 출력
            }
            else {
                nextPage().next_page();//다음페이지 출력하고
                consol().con();
            }
            
        }
        else if (s == "p") {//이전페이지                                   
            p_cnt--;
            if (fp - 20 < 0 ) {
                previousPage().pre_page();
                consol().first_con();
            }
            if (p_cnt+n_cnt < 0) {//현재 페이지와 이전 페이지를 누른 수의 합이 - 라면             
                p_cnt++;
            }
            else {
                previousPage().pre_page();
                consol().con();
            }
        }
        else if (s.front() == 'i') {//삽입                        
            insert().insert_word(s);
            nextPage().cur_page();
            consol().con();
        }
        else if (s.front() == 'd') {//삭제
            del().delete_word(s);
            nextPage().cur_page();
            consol().con();
        }
        else if (s.front() == 'c') {//변경                           
            change().change_word(s);//변경하고                                    
            nextPage().cur_page();//다시 멈춰 있던 페이지 출력하고
            consol().con();//콘솔창 출력

        }
        else if (s.front() == 's') {//찾기       
            search().search_word(s);
            if (isFind == 0) {//단어 있을경우
                nextPage().cur_page();
                consol().con();//정상출력
            }
            else {//단어 없을경우
                nextPage().cur_page();
                consol().noWord_con();//다시 출력
            }
        }
        else if (s.front() == 't') {//저장 후 종료       
            save().close();
            return 0;
        }
        cnt++;
    }
    return 0;
}