#include <iostream>

struct node
{
    int value;  // Value of the node
    node *prev; // Address of the previous node
    node *next; // Address of the previous node
};

class DoublyLinkedList
{
private:
    node *head; // Fisrt node in the list
    int size;   // Size of the list

    // Get a new node
    node *getNewNode(int value)
    {
        node *newNode = new node();
        newNode->value = value;
        newNode->next = NULL;
        newNode->prev = NULL;
        return newNode;
    }

public:
    // Create an empty list
    DoublyLinkedList()
    {
        head = NULL;
        size = 0;
    }

    // Add a node to the list
    void add(int value)
    {
        node *n = getNewNode(value);
        size++;

        if (head == NULL)
        {
            head = n;
            return;
        }

        node *last = head;

        while (last->next != NULL)
            last = last->next;

        last->next = n;
        n->prev = last;
    }

    // Get a node its value
    int get(int index)
    {
        node *n = head;

        while (index-- > 0)
            n = n->next;

        return n->value;
    }

    // Remove a node from the list
    void remove(int index)
    {
        node *n = head;
        size--;

        if (index == 0)
        {
            if (head->next != NULL)
            {
                head = n->next;
                head->prev = NULL;
                delete n;
            }
            else
            {
                delete n;
                head = NULL;
            }
            return;
        }

        while (index-- > 0)
            n = n->next;

        if (n->next == NULL)
        {
            n->prev->next = NULL;
            delete n;
        }
        else
        {
            n->prev->next = n->next;
            n->next->prev = n->prev;
            delete n;
        }
    }

    // Remove all nodes from the list
    void empty()
    {
        node *n;
        size = 0;

        while (head != NULL)
        {
            n = head;
            head = head->next;
            delete n;
        }
    }

    // Get the size of the list
    int len()
    {
        return size;
    }
};

void loop(int, int);

DoublyLinkedList num;

int main()
{
    char input[] = "15";

    {
        int size = 0;
        do
            num.add(input[size] - '0');
        while (input[++size] != '\0');
    }

    for (int i = 0; i < num.len(); i++)
    {
        loop(num.get(i), num.len() - i - 1);
    }
}

void loop(int value, int times)
{
    if (times == 0)
        std::cout << value << std::endl;
    else
        for (int i = 0; i < 10; i++)
            loop(value, times - 1);
}