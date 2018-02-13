// CIS-22B
// Group Project
//
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>

using namespace std;

class PriceList
{
    private:
        struct ListNode
        {
            // Price list
            double price;
            struct ListNode *next;
        };
        
    public:
        ListNode *head;
        PriceList()
        {
            head = nullptr;
        }
        
        void appendList(double d);
        double displayList(int i);
};

struct CustomerIDs
{
    int seat;
    int row;
    string cID;
};

void addTickets(PriceList prices, bool finish, int tickets, double total, int tID, string name, string cID, char first[30], char last[30]);
void deleteTickets(PriceList prices);
void displaySeats();
void displaySeats(CustomerIDs *cIDs, int elements);
void searchRecord();
void totalSales();
void searchCustomerIDs(int tID, CustomerIDs *answer, int &elements);
void setPrices(PriceList &prices);
string idMaker(char first[30], char last[30], int num1, int num2);
bool checkAvailable(int row, int seat);

int main()
{
    int choice;
    char c[30], c2[30];
    bool finish = false;
    
    PriceList prices;
    setPrices(prices);
    
    while(!finish)
    {
        cout << endl << "Enter the number of your choice" << endl;
        cout << "1. Purchase Tickets" << endl;
        cout << "2. Cancel Tickets" << endl;
        cout << "3. Display Available Seats" << endl;
        cout << "4. Search Record" << endl;
        cout << "5. Total Ticket Sales" << endl;
        cout << endl << "0. Quit" << endl;
        cout << endl << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        
        switch(choice)
        {
            case 0:
                finish = true;
                break;
            case 1:
                addTickets(prices, false, 0, 0, 0, "", "", c, c2);
                break;
                
            case 2:
                deleteTickets(prices);
                break;
                
            case 3:
                displaySeats();
                break;
                
            case 4:
                searchRecord();
                break;
                
            case 5:
                totalSales();
                break;
                
            default:
                cout << "Error" << endl;
                break;
        }
        
        if(choice != 0)
        {
            cout << endl << "Please enter any key...";
            cin.ignore();
            cin.get();
            cout << endl;
        }
    }
    return 0;
}

void addTickets(PriceList prices, bool finish, int tickets, double total, int tID, string name, string cID, char first[30], char last[30])
{
    int row, seat, choice;
    string line, update;
    
    if(tickets >= 10)
    {
        cout << "Maximum number of purchasing tickets is 10 per customer." << endl;
        cout << "Total: $" << fixed << setprecision(2) << total << endl;
        
        cout << "Transaction ID: " << tID << endl;
        
        ifstream ifs2("CustomerDatabase.txt");
        
        while (getline(ifs2, line))
        {
            update += line;
            update += "\n";
        }
        
        ofstream ofs2("CustomerDatabase.txt");
        ofs2 << update;
        ofs2 << tID << " " << cID << endl;
        ofs2 << name << endl;
        ofs2 << tickets << endl;
        ofs2 << total << endl;
        
        ifs2.close();
        ofs2.close();
        
        return;
    }
    else if(finish)
    {
        cout << "Total: $" << fixed << setprecision(2) << total << endl;
        cout << "Transaction ID: " << tID << endl;
        
        ifstream ifs2("CustomerDatabase.txt");
        
        while (getline(ifs2, line))
        {
            update += line;
            update += "\n";
        }
        
        ofstream ofs2("CustomerDatabase.txt");
        ofs2 << update;
        ofs2 << tID << " " << cID << endl;
        ofs2 << name << endl;
        ofs2 << tickets << endl;
        ofs2 << total << endl;
        
        ifs2.close();
        ofs2.close();
        
        return;
    }
    else
    {
        if(tickets == 0)
        {
            cout << "Enter your first name: ";
            cin >> first;
            cout << "Enter your last name: ";
            cin >> last;
            
            name = first;
            name += " ";
            name += last;

            cout << endl << "Hello, " << first << " " << last << endl;
        }
        
        cout << "Here are the seats available" << endl << endl;
        displaySeats();
        
        cout << endl << "Enter the row number" << endl;
        cin >> row;
        cout << "Enter the seat number" << endl;
        cin >> seat;
        
        if(!checkAvailable(row, seat))
        {
            while (!checkAvailable(row, seat))
            {
                cout << endl << "This seat is unavailable. Please enter again." << endl;
                cout << endl << "Enter the row number" << endl;
                cin >> row;
                cout << "Enter the seat number" << endl;
                cin >> seat;
            }
        }
        
        cout << endl << "Seat Reserved." << endl;
        cout << "Row: " << row << "     Seat: " << seat << endl;
        cout << "Price: $" << fixed << setprecision(2) << prices.displayList(row) << endl;
        
        cout << endl << "Would you like to reserve any more seats?" << endl;
        cout << "1: Yes     2: No" << endl;
        cin >> choice;
        if(choice == 2)
        {
            finish = true;
        }
        
        int temp;
        string str;
        sscanf(idMaker(first, last, row, seat).c_str(), "%d", &temp);
        tID += temp;
        cID += idMaker(first, last, row, seat).substr(0, idMaker(first, last, row, seat).length()-1);
        cID += " ";
        total += prices.displayList(row);
        tickets++;
        
        ifstream ifs("SeatingChart.txt");
        while(getline(ifs,line))
        {
            update += line;
            update += "\n";
            int numR, numS;
            sscanf(line.c_str(), "%d %d", &numR, &numS);
            if((numR == row) && (numS == seat))
            {
                update += idMaker(first, last, row, seat);
                update += name;
                update += "\n\n";
                getline(ifs,line);
                getline(ifs,line);
                getline(ifs,line);
            }
            else
            {
                getline(ifs,line);
                update += line + "\n";
                getline(ifs,line);
                update += line + "\n";
                getline(ifs,line);
                update += line + "\n";
            }
        }
        
        ofstream ofs("SeatingChart.txt");
        ofs << update;
        
        ifs.close();
        ofs.close();
        
        cout << endl;
        addTickets(prices, finish, tickets, total, tID, name, cID, first, last);
    }
}

void deleteTickets(PriceList prices)
{
    int tID, elements = 0, choice, row, seat, temp, space = 0, temp2, num;
    double numd;
    string line, update, update2, str;
    CustomerIDs cIDs[10];
    
    cout << "Enter your Transaction ID: ";
    cin >> tID;
    
    searchCustomerIDs(tID, cIDs, elements);
    
    cout << endl << "Your seats: " << endl << endl;
    
    displaySeats(cIDs, elements);
    
    for(int i = 0; i < elements; i++)
    {
        cout << "   #" << i+1 << " - Row: " << cIDs[i].row << "     Seat: " << cIDs[i].seat << endl;
    }
    
    cout << endl << "Enter the number of deleting seat: ";
    cin >> choice;
    
    ifstream ifs("SeatingChart.txt");
    
    while(getline(ifs, line))
    {
        sscanf(line.c_str(), "%d %d", &row, &seat);
        if((row == cIDs[choice-1].row) && (seat == cIDs[choice-1].seat))
        {
            update += line + "\n";
            update += "0";
            update += "\n";
            update += "\n";
            update += "\n";
            getline(ifs, line);
            getline(ifs, line);
            getline(ifs, line);
        }
        else
        {
            update += line + "\n";
            getline(ifs, line);
            update += line + "\n";
            getline(ifs, line);
            update += line + "\n";
            getline(ifs, line);
            update += line + "\n";
        }
        
    }
        
    ifs.close();
    
    ofstream ofs("SeatingChart.txt");
    ofs << update;
    ofs.close();
    
    ifstream ifs2("CustomerDatabase.txt");
    
    while(getline(ifs2, line))
    {
        sscanf(line.c_str(), "%d", &temp);
        if(tID == temp)
        {
            while((temp2 = line.find(" ", space)) >= 0)
            {
                if((line.substr(space, temp2-space).compare(cIDs[choice-1].cID)) != 0)
                {
                    update2 += line.substr(space, temp2-space) + " ";
                }
                
                space = temp2+1;
            }
            update2 += "\n";
            getline(ifs2, line);
            update2 += line + "\n";
            getline(ifs2, line);
            sscanf(line.c_str(), "%d", &num);
            ostringstream oss;
            oss << num-1;
            update2 += oss.str() + "\n";
            getline(ifs2, line);
            sscanf(line.c_str(), "%lf", &numd);
            ostringstream oss2;
            oss2 << numd-prices.displayList(cIDs[choice-1].row);
            update2 += oss2.str() + "\n";
        }
        else
        {
            update2 += line + "\n";
            getline(ifs2, line);
            update2 += line + "\n";
            getline(ifs2, line);
            update2 += line + "\n";
            getline(ifs2, line);
            update2 += line + "\n";
        }
    }
    
    ifs2.close();
    
    ofstream ofs2("CustomerDatabase.txt");
    ofs2 << update2;
    ofs2.close();
}

void displaySeats()
{
    cout.width(6);
    cout << "";
    for(int i = 1; i <= 30; i++)
    {
        cout.width(3);
        cout << right << i;
    }
    cout << endl;
    
    ifstream ifs("SeatingChart.txt");
    string line;
    int seat, row, id, count=1;
    while(getline(ifs,line))
    {
        if(count%30 == 1)
        {
            cout << "Row";
            cout.width(3);
            cout << right << (count/30)+1;
        }
        getline(ifs,line);
        sscanf(line.c_str(), "%d", &id);
        if(id == 0)
        {
            cout.width(3);
            cout << "_";
        }
        else
        {
            cout.width(3);
            cout << "x";
        }
        
        if(count%30 == 0) cout << "\n";
        
        getline(ifs,line);
        getline(ifs,line);
        count++;
    }
    
    cout << endl << endl;
    cout.width(30);
    cout << right << "" << "_ = seats available" << endl;
    cout.width(30);
    cout << right << "" << "x = seats unavailable" << endl;
    ifs.close();
}

void displaySeats(CustomerIDs *cIDs, int elements)
{
    cout.width(6);
    cout << "";
    for(int i = 1; i <= 30; i++)
    {
        cout.width(3);
        cout << right << i;
    }
    cout << endl;
    
    ifstream ifs("SeatingChart.txt");
    string line;
    int seat, row, id, count=1;
    while(getline(ifs,line))
    {
        bool tf = false;
        if(count%30 == 1)
        {
            cout << "Row";
            cout.width(3);
            cout << right << (count/30)+1;
        }
        sscanf(line.c_str(), "%d %d", &row, &seat);
        for(int i = 0; i < elements; i++)
        {
            if((row == cIDs[i].row) && (seat == cIDs[i].seat))
            {
                cout.width(2);
                cout << "#" << i+1;
                tf = true;
                break;
            }
            
            
        }
        
        if(!tf)
        {
            cout.width(3);
            cout << "_";
        }
        
        if(count%30 == 0) cout << "\n";
        
        getline(ifs,line);
        getline(ifs,line);
        getline(ifs,line);
        count++;
    }
    
    cout << endl << endl;
    cout.width(30);
    cout << right << "" << "# = Your seats" << endl;
    cout.width(30);
    cout << right << "" << "_ = Not your seats" << endl << endl;
    ifs.close();
}

void searchRecord()
{
    int tID, temp, temp2, num, space = 0, count = 0, cIDs[10], row, seat;
    string line, name;
    CustomerIDs seatNumbers[10];
    
    cout << "Enter your Transaction ID: ";
    cin >> tID;
    
    ifstream ifs("CustomerDatabase.txt");
    
    while(getline(ifs, line))
    {
        sscanf(line.c_str(), "%d", &temp);
        if(tID == temp)
        {
            while((temp2 = line.find(" ", space)) >= 0)
            {
                sscanf((line.substr(space, temp2-space)).c_str(), "%d", &num);
                if(count > 0)
                {
                    cIDs[count-1] = num;
                    count++;
                }
                else
                {
                    count++;
                }
                
                space = temp2+1;
            }
        }
    }
    
    count--;
    ifs.close();
    
    for(int i = 0; i < count; i++)
    {
        ifstream ifs2("SeatingChart.txt");
        
        while(getline(ifs2, line))
        {
            sscanf(line.c_str(), "%d %d", &row, &seat);
            
            getline(ifs2, line);
            sscanf(line.c_str(), "%d", &temp);
            if(temp == cIDs[i])
            {
                seatNumbers[i].row = row;
                seatNumbers[i].seat = seat;
                
                if(i == 0)
                {
                    getline(ifs2, line);
                    name = line;
                }
                else
                {
                    getline(ifs2, line);
                }
            }
            else
            {
                getline(ifs2, line);
            }
            
            getline(ifs2, line);
        }
        
        ifs2.close();
    }
    
    
    
    cout << endl << "Customer Name: " << name << endl << endl;
    
    for(int i = 0; i < count; i++)
    {
        cout << "   #" << i+1 << " - Row: " << seatNumbers[i].row << "   Seat: " << seatNumbers[i].seat << endl;
    }
}

void totalSales()
{
    double answer = 0, temp;
    string line;
    
    ifstream ifs("CustomerDatabase.txt");
    
    while(getline(ifs, line))
    {
        getline(ifs, line);
        getline(ifs, line);
        getline(ifs, line);
        sscanf(line.c_str(), "%lf", &temp);
        answer += temp;
    }
    
    ifs.close();
    
    cout << "Total Sales: $" << fixed << setprecision(2) << answer;
}

void setPrices(PriceList &prices)
{
    string line;
    double price;
        
    ifstream ifs("SeatPrices.txt");
    while(getline(ifs, line))
    {
        sscanf(line.c_str(), "%lf", &price);
        prices.appendList(price);
    }
}

string idMaker(char first[30], char last[30], int num1, int num2)
{
    string answer;
    int num = 0;
    
    for(int i = 0; i < strlen(first); i++)
    {
        num += first[i];
    }
    
    if((num/1000) == 0)
    {
        num += (num1%10)*1000;
    }
    else
    {
        num += num1%10;
    }
    
    ostringstream oss;
    oss << num;
    
    answer += oss.str();
    num = 0;
    
    for(int i = 0; i < strlen(last); i++)
    {
        num += last[i];
    }
    
    if((num/1000) == 0)
    {
        num += (num2%10)*1000;
    }
    else
    {
        num += num2%10;
    }
    
    ostringstream oss2;
    oss2 << num;
    
    answer += oss2.str();
    answer += "\n";
    
    return answer;
}

bool checkAvailable(int row, int seat)
{
    bool answer = false;
    string line;
    int numR, numS, id;
        
    ifstream ifs("SeatingChart.txt");
    while(getline(ifs, line))
    {
        sscanf(line.c_str(), "%d %d", &numR, &numS);
        if((numR == row) && (numS == seat))
        {
            getline(ifs, line);
            sscanf(line.c_str(), "%d", &id);
            break;
        }
        else
        {
            getline(ifs, line);
            getline(ifs, line);
            getline(ifs, line);
        }
    }
    
    if(id == 0)
    {
        answer = true;
    }
    else
    {
        answer = false;
    }
    
    return answer;
}

void searchCustomerIDs(int tID, CustomerIDs *answer, int &elements)
{
    string line;
    int temp, space = 0, count = 0, seat, row;
    
    ifstream ifs("CustomerDatabase.txt");
    
    while(getline(ifs, line))
    {
        sscanf(line.c_str(), "%d", &temp);
        if(tID == temp)
        {
            for(int i = 0; i < line.length(); i++)
            {
                space = line.find(" ", i);
                if(space == -1)
                {
                    answer[count-1].cID = line.substr(i, line.length()-i);
                    break;
                }
                
                if(count > 0)
                {
                    answer[count-1].cID = line.substr(i, space-i);
                }
                count++;
                i = space++;
            }
        }
        getline(ifs, line);
        getline(ifs, line);
        getline(ifs, line);
    }
    
    ifs.close();
    
    elements = count-1;
    
    for(int i = 0; i < count-1; i++)
    {
        ifstream ifs2("SeatingChart.txt");
        
        while(getline(ifs2, line))
        {
            sscanf(line.c_str(), "%d %d", &row, &seat);
            getline(ifs2, line);
            if(line == answer[i].cID)
            {
                answer[i].row = row;
                answer[i].seat = seat;
            }
        }
        
        ifs2.close();
    }
}

void PriceList::appendList(double d)
{
    ListNode *newNode;
    ListNode *nodePtr;
    
    newNode = new ListNode;
    newNode->price = d;
    newNode->next = nullptr;
    
    if(!head)
    {
        head = newNode;
    }
    else
    {
        nodePtr = head;
        
        while(nodePtr->next)
        {
            nodePtr = nodePtr->next;
        }
        
        nodePtr->next = newNode;
    }
}

double PriceList::displayList(int i)
{
    ListNode *nodePtr;
    ListNode *previousNode;
    
    nodePtr = head;
    
    for(int j = 1; j < i; j++)
    {
        previousNode = nodePtr;
        nodePtr = nodePtr->next;
    }
    
    return nodePtr->price;
}