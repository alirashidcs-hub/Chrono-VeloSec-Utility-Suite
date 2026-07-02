#include<iostream>
#include<string>
#include<ctime>
#include<iomanip>
#include<cctype>
#include<cstdlib>
#include<vector>
#include<limits>
#include<windows.h>
#include<conio.h>

using namespace std;

class ScreenUtility 
{
public:
    static void clearScreen() { system("cls"); }
    static void crossSleep(int ms) { Sleep(ms); }
};

class PasswordChecker 
{
private:
    string pwd;
    int score;
    bool hasUpper, hasLower, hasDigit, hasSymbol;
public:
    PasswordChecker() : score(0), hasUpper(false), hasLower(false), hasDigit(false), hasSymbol(false) {}

    void displayHeader() {
        cout << "============================================================\n";
        cout << "=$            SECURITY: PASSWORD STRENGTH CHECK           $=\n";
        cout << "============================================================\n";
        cout << "~Characteristics/Rules:\n-More than 8 Characters\n-Uppercase\n-Lowercase\n-Numbers\n-Symbols\n\n";
    }

    void getPassword() 
	{
        cout << "Enter Password: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, pwd);
    }

    void analyzePassword() 
	{
        score = 0;
        hasUpper = hasLower = hasDigit = hasSymbol = false;
        if (pwd.length() >= 8) score++;
        for (char c : pwd) 
		{
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
            if (ispunct(c)) hasSymbol = true;
        }
        if (hasUpper) score++;
        if (hasLower) score++;
        if (hasDigit) score++;
        if (hasSymbol) score++;
    }

    void displayResult() 
	{
        cout << "\nAnalysis Result: " << score << "/5\n";
        if (score <= 1) cout << "STATUS: [Brutal - Its not a thing called password....]\n";
        else if (score == 2) cout << "STATUS: [Very Weak - Read the rules above....]\n";
        else if (score == 3) cout << "STATUS: [Weak - Need Improvement, Follow instructions...]\n";
        else if (score == 4) cout << "STATUS: [Strong - Welldone!!!]\n";
        else cout << "STATUS: [Best - Full Encryption!!!]\n";
    }

    void execute() 
	{
        ScreenUtility::clearScreen();
        displayHeader();
        getPassword();
        analyzePassword();
        displayResult();
    }
};

class TypingTester 
{
private:
    int mode, level, timeLimit;
    string targetParagraph, reference, input;
    double elapsed;
    int correctChars, droppedChars, extraChars, errors, caughtChars, missedChars;
    double accuracy, cpm, wpm;
    
    struct FallingChar 
	{
        char ch;
        int x, y;
        FallingChar(char c, int xpos, int ypos) : ch(c), x(xpos), y(ypos) {}
    };
    vector<FallingChar> fallingChars;
    
    vector<string> oopParagraphs = {
        "Object-Oriented Programming is a paradigm based on classes and objects. It uses encapsulation to bundle data and methods, inheritance to promote code reuse, and polymorphism to allow flexible interfaces. Abstraction hides complex implementation details from users.",
        "A class is a blueprint for creating objects. It defines attributes as data members and behaviors as member functions. Objects are instances of classes that maintain state and exhibit behavior through method calls and message passing in C++.",
        "Encapsulation protects data by restricting direct access to private members. They can only be accessed through public getter and setter methods. This ensures data integrity and allows controlled modification of object state in programs."
    };
    vector<string> oopSources = {
        "Source: OOP Fundamentals - UET CS Dept",
        "Source: C++ Classes & Objects Guide", 
        "Source: Encapsulation Best Practices"
    };

    void gotoxy(int x, int y) 
	{
        COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void selectMode() 
	{
        cout << "============================================================\n";
        cout << "=$               ALARM: TYPING SPEED TESTER               $=\n";
        cout << "============================================================\n";
        cout << "~ ~ ~ ~ SELECT TEST MODE ~ ~ ~ ~\n";
        cout << "1. Paragraph Mode (OOP Concepts Based)\n";
        cout << "2. Character Drop Mode (Game Style)\n";
        cout << "Choice: ";
        cin >> mode;
    }

    void selectParagraphDifficulty() 
	{
        cout << "\n~ ~ ~ SELECT OOP TOPIC ~ ~ ~\n";
        cout << "1. OOP Fundamentals\n";
        cout << "2. Classes & Objects\n";
        cout << "3. Encapsulation\n";
        cout << "Choice: ";
        cin >> level;
        if(level >= 1 && level <= 3) {
            targetParagraph = oopParagraphs[level-1];
            reference = oopSources[level-1];
        } else {
            targetParagraph = oopParagraphs[0];
            reference = oopSources[0];
        }
    }

    void displayParagraph() 
	{
        cout << "\n                    <-------------------------------------------------------------------->\n";
        cout << "Text to Type ( OOP-Based Paragraph ): \n\t\t\t" << targetParagraph << "\n(" << reference << ")\n";
        cout << "\n\nNote: Once Typed, Can't Undo !!!, Even Spaces matter... \n";
        cout << "                    <-------------------------------------------------------------------->\n\n";
    }

    void clearFallingChars() 
	{
        for(auto &fc : fallingChars) 
		{
            if(fc.y >= 2 && fc.y <= 22) 
			{
                gotoxy(fc.x, fc.y);
                cout << " ";
            }
        }
        cout.flush();
    }

    void displayFallingChars() 
	{
        for(auto &fc : fallingChars) 
		{
            if(fc.y >= 2 && fc.y <= 22) 
			{
                gotoxy(fc.x, fc.y);
                cout << fc.ch;
            }
        }
        cout.flush();
    }

    FallingChar generateRandomChar() 
	{
        char ch;
        if(rand() % 2 == 0) 
            ch = static_cast<char>(rand() % 26 + 65); 
        else 
            ch = static_cast<char>(rand() % 26 + 97); 
        int x = rand() % 50 + 15; 
        int y = 2; 
        return FallingChar(ch, x, y);
    }
	
    void runCharacterDropMode() 
	{
        ScreenUtility::clearScreen();
        cout << "============================================================\n";
        cout << "=$          CHARACTER DROP: TYPING GAME MODE            $=\n";
        cout << "============================================================\n";
        cout << "Characters fall randomly! Type exact character to catch it.\n";
        cout << "Mix of Capital & Small letters. Max 3 on screen at once.\n\n";
        
        cout << "Enter Game Duration (seconds): ";
        cin >> timeLimit;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "\nGet Ready... Press ENTER to START!\n";
        cin.get();
        
        ScreenUtility::clearScreen();
        cout << "=== CATCH THE FALLING CHARACTERS === (Time: " << timeLimit << "s)\n";
        cout << "----------------------------------------\n\n";
        cout << "[Tips: Type exact character (case-sensitive) to catch!]\n\n";
        
        input = "";
        time_t startTime = time(0);
        elapsed = 0;
        caughtChars = 0;
        missedChars = 0;
        fallingChars.clear();
        
        srand(static_cast<unsigned>(time(0)));
        int spawnCounter = 0;
        const int spawnCheckInterval = 10; 
        
        while(true) 
		{
            elapsed = difftime(time(0), startTime);
            
            if(elapsed >= timeLimit) 
			{
                clearFallingChars();
                cout << "\n\n!!! TIME UP !!! Game Over 🎮\n";
                for(int i=0; i<3; i++) { Beep(750, 200); ScreenUtility::crossSleep(150); }
                break;
            }
            
            spawnCounter++;
            if(spawnCounter >= spawnCheckInterval && fallingChars.size() < 3) {
                spawnCounter = 0;
                int toSpawn = rand() % 2 + 2; 
                for(int s=0; s<toSpawn && fallingChars.size()<3; s++) {
                    fallingChars.push_back(generateRandomChar());
                }
            }
            
            clearFallingChars();
            for(auto it = fallingChars.begin(); it != fallingChars.end(); ) {
                it->y++; 
          
                if(it->y > 22) {
                    missedChars++;
                    it = fallingChars.erase(it);
                } else {
                    ++it;
                }
            }
            
            displayFallingChars();
            
            if(_kbhit()) {
                char ch = _getch();
             
                if(isalpha(ch)) {
                    cout << ch; 
                    input += ch;
                    
                    for(auto it = fallingChars.begin(); it != fallingChars.end(); ++it) {
                        if(it->ch == ch) {
                            gotoxy(it->x, it->y);
                            cout << " ";
                            caughtChars++;
                            fallingChars.erase(it);
                            break; 
                        }
                    }
                }
                else if(ch == 8 && !input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            
            ScreenUtility::crossSleep(100);
        }
        
        gotoxy(0, 26);
        ScreenUtility::crossSleep(10);
    }

    void getTimeLimit() {
        cout << "Enter Maximum Time (Seconds): ";
        cin >> timeLimit;
    }

    void captureInput() {
        cout << "Press Enter To Begin and Finish...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        input = "";
        time_t startTime = time(0);
        elapsed = 0;
        
        cout << "\nStart Typing !!! : \n\n";
        while(true) {
            elapsed = difftime(time(0), startTime);
            if(elapsed >= timeLimit) {
                cout << "\n\n!!! ALARM: TIME LIMIT EXCEEDED !!!\n";
                for(int i=0; i<3; i++) { Beep(750,300); ScreenUtility::crossSleep(200); }
                break;
            }
            if(_kbhit()) {
                char ch = _getch();
                if(ch == 13) break; 
                else if(ch == 8) { 
                    if(!input.empty()) { input.pop_back(); cout << "\b \b"; }
                } else { input += ch; cout << ch; }
            }
        }
        ScreenUtility::crossSleep(10);
    }

    void calculateParagraphResults() {
        correctChars = droppedChars = extraChars = errors = 0;
        size_t tLen = targetParagraph.length(), iLen = input.length();
        
        for(size_t i=0; i<tLen; i++) {
            if(i < iLen) {
                if(targetParagraph[i] == input[i]) correctChars++;
                else errors++;
            } else {
                droppedChars++; 
            }
        }
        if(iLen > tLen) extraChars = iLen - tLen; 
        
        accuracy = (tLen > 0) ? ((double)correctChars / tLen) * 100.0 : 0.0;
        if(accuracy < 0) accuracy = 0;
        if(accuracy > 100) accuracy = 100;
        
        double minutes = elapsed / 60.0;
        cpm = (minutes > 0) ? correctChars / minutes : 0.0;
        wpm = cpm / 5.0;
    }

    void calculateDropModeResults() {
        int totalChars = caughtChars + missedChars;
        accuracy = (totalChars > 0) ? ((double)caughtChars / totalChars) * 100.0 : 0.0;
        if(accuracy < 0) accuracy = 0;
        if(accuracy > 100) accuracy = 100;
        
        double minutes = elapsed / 60.0;
        cpm = (minutes > 0) ? caughtChars / minutes : 0.0;
        wpm = cpm / 5.0;
        droppedChars = missedChars;
        correctChars = caughtChars;
    }

    void displayResults() {
        cout << "\n\n---TEST RESULT---\n";
        cout << "Time Used: " << fixed << setprecision(1) << elapsed << "s / " << timeLimit << "s\n";
        cout << "Accuracy: " << setprecision(2) << accuracy << "%\n";
        
        if(mode == 1) {
            cout << "Characters Typed: " << input.length() << " | Correct: " << correctChars 
                 << " | Dropped: " << droppedChars << " | Extra: " << extraChars << "\n";
            cout << "CPM: " << setprecision(1) << cpm << " | WPM: " << setprecision(1) << wpm << "\n";
            cout << "Your Input: \n" << input << "\n";
        } 
        else { 
            cout << "Characters Caught: " << caughtChars << " | Missed/Dropped: " << missedChars << "\n";
            cout << "Total Attempted: " << (caughtChars + missedChars) << "\n";
            cout << "CPM (Caught/Min): " << setprecision(1) << cpm << " | WPM: " << setprecision(1) << wpm << "\n";
            cout << "Typed Sequence: " << input << "\n";
        }
    }

public:
    TypingTester() : mode(1), level(1), timeLimit(0), elapsed(0), 
                     correctChars(0), droppedChars(0), extraChars(0), errors(0),
                     caughtChars(0), missedChars(0), accuracy(0), cpm(0), wpm(0) {}

    void execute() 
	{
        ScreenUtility::clearScreen();
        selectMode();
        
        if(mode == 1) 
		{
            selectParagraphDifficulty();
            displayParagraph();
            getTimeLimit();
            captureInput();
            calculateParagraphResults();
        } 
        else if(mode == 2)			
		{
            runCharacterDropMode();
            calculateDropModeResults();
        }
        
        displayResults();
    }
};
class ChronosVeloSecSuite 
{
private:
    PasswordChecker pwdChecker;
    TypingTester typingTester;
    int choice;

    void displayBanner() 
	{
    ScreenUtility::clearScreen();
    cout << "\n\n";
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                     *  *  *  *  *  *  *  *  *  *  *  *                   )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                             D e p a r t m e n t                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                      *  *  *  *  *  *  *  *  *  *  *  *                  )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(300);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                    o f                                   )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(300);
    cout << "(                     *  *  *  *  *  *  *  *  *  *  *  *                   )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                       C o m p u t e r   S c i e n c e                    )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                      *  *  *  *  *  *  *  *  *  *  *  *                  )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(500);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                   ============================================           )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                  ||  UNIVERSITY OF ENGINEERING & TECHNOLOGY  ||          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                   ============================================           )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(2000);
    
	
    ScreenUtility::clearScreen();
    cout << "\n\n\n\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                    @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @             )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                          @  A   G R O U P   O F   3  @                   )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                    @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @             )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(1500); 

    
    ScreenUtility::clearScreen();
    cout << "\n\n\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                    @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @             )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                               @  TEAM  MEMBERS  @                        )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                    @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @             )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                      ~  Rana Farrid          ~  25-CS-193                )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                      ~  Chaudhry Ali Rashid  ~  25-CS-119                )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                      ~  Yasoob Khan          ~  25-CS-131                )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";    
    ScreenUtility::crossSleep(2300); 
     
	 
    ScreenUtility::clearScreen();
    cout << "\n\n\n\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";    
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                         $  $  $  $  $  $  $  $                           )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                             P R E S E N T S                              )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                         $  $  $  $  $  $  $  $                           )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                          )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";    
    ScreenUtility::crossSleep(1000);
    
   
    ScreenUtility::clearScreen();
    cout << "\n\n\n";
    ScreenUtility::crossSleep(100);
	 cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n\n";    
    ScreenUtility::crossSleep(800);
    cout << "(    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    ||    $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $     ||      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    ||                                                             ||      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    ||               C H R O N O S  -  V E L O S E C               ||      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    ||                  U T I L I T Y   S U I T E                  ||      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    ||                                                             ||      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    ||    $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $     ||      )\n";
    ScreenUtility::crossSleep(100);
    cout << "(    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++      )\n";
    ScreenUtility::crossSleep(800);
    cout << "\n";
    ScreenUtility::crossSleep(100);
    cout << "(                   $  $  $  OOP Semester Project  $  $  $                  )\n";
    ScreenUtility::crossSleep(2000); 
    cout << "\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                           )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                      !  Press ENTER to Continue...  !                     )\n";
    ScreenUtility::crossSleep(100);
    cout << "(                                                                           )\n";
    ScreenUtility::crossSleep(100);
    cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n";    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get(); 

    }

    void displayExplanation() 
    {
    ScreenUtility::clearScreen();
    cout << "================================================================\n";
    ScreenUtility::crossSleep(100);
    cout << "                            EXPLANATION                         \n";
    ScreenUtility::crossSleep(100);
    cout << "================================================================\n";
    ScreenUtility::crossSleep(100);
    cout << "\n % DERIVATION:\n";
    ScreenUtility::crossSleep(100);
    cout << "   =============================================                \n";
    ScreenUtility::crossSleep(100);
    cout << "  | [Chronos] - Greek God of Time & Precision   |               \n";
    ScreenUtility::crossSleep(100);
    cout << "  | [VeloSec] - Velocity + Security Hybrid      |               \n";
    ScreenUtility::crossSleep(100);
    cout << "   =============================================                \n";
    ScreenUtility::crossSleep(100);
    cout << "\n % PURPOSE:\n";
    ScreenUtility::crossSleep(100);
    cout << "   To evaluate typing velocity & password strength              \n";
    ScreenUtility::crossSleep(100);
    cout << "   within a time-synchronized, OOP-based environment.           \n";
    ScreenUtility::crossSleep(100);
    cout << "\n % MODULES / TYPES:\n";
    ScreenUtility::crossSleep(100);
    cout << "   ===============================================                  \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |    TYPING SPEED TESTER                        |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |    - Mode 1: Paragraph Mode                   |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |   |   - OOP-based content                     |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |   |   - Time-limited typing                   |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |   |   - Accuracy + CPM/WPM analysis           |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |   |                                           |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |    - Mode 2: Character Drop Game              |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |       - Falling characters (A-Z, a-z)         |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |       - Catch by typing exact match           |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |      - Real-time caught/missed tracking       |                 \n";
    ScreenUtility::crossSleep(100);                            
    cout << "  |                                               |                 \n";
    ScreenUtility::crossSleep(100);                           
    cout << "  |    PASSWORD STRENGTH CHECKER                  |                 \n";
    ScreenUtility::crossSleep(100);                           
    cout << "  |      - 5-point scoring system                 |                 \n";
    ScreenUtility::crossSleep(100);                           
    cout << "  |      - Checks: Length, Case, Digits, Symbols  |                 \n";
    ScreenUtility::crossSleep(100);                                             
    cout << "  |      - Status: Brutal → Weak → Strong → Best  |                 \n";
    ScreenUtility::crossSleep(100);                                             
    cout << "   ===============================================                  \n";
    ScreenUtility::crossSleep(100);
    cout << "\n % TECH STACK:\n";
    ScreenUtility::crossSleep(100);
    cout << "   OOP - C++ - Windows API - Real-time Input - ANSI Art\n";
    ScreenUtility::crossSleep(100);
    
    cout << "\n================================================================\n";
    ScreenUtility::crossSleep(100);
    cout << "       Tip: Use exact case in Character Drop Mode! \n";
    ScreenUtility::crossSleep(100);
    cout << "================================================================\n";
    
    cout << "\n\n   `  Press Enter to Continue...  `\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    }

    void displayMenu() 
	{
        ScreenUtility::clearScreen();
        time_t now = time(0);
        cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
        cout << "                Current System Time: " << ctime(&now);
        cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
        cout << "\n1. Typing Speed Tester \n";
        cout << "2. Check Password Security Strength\n";
        cout << "3. Exit Program\n";
        cout << "\nEnter Choice {1,2,3}: ";
    }

    bool handleChoice() 
	{
        if (!(cin >> choice)) 
		{
            cout << "Invalid Input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ScreenUtility::crossSleep(2000);
            return true;
        }

        if (choice == 1) typingTester.execute();
        else if (choice == 2) pwdChecker.execute();
        else if (choice == 3) 
		{
            cout << "Closing Suite... Stay Secure! Jazakallah 🤍\n";
            return false;
        } 
		else 
		{
            cout << "Option not found!\n";
        }

        cout << "\nPress Enter to return to main menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return true;
    }

public:
    ChronosVeloSecSuite() : choice(0) {}
    void run() 
	{
        displayBanner();
        displayExplanation();
        while (true) 
		{
            displayMenu();
            if (!handleChoice()) break;
        }
    }
};

int main() 
{
	system("cls");
    ChronosVeloSecSuite suite;
    suite.run();
    return 0;
}