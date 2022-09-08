#include <iostream>
#include <vector>

using namespace std;

struct SegmentTree
{

public:
    explicit SegmentTree(const vector<int>& a) : SIZE(a.size())
    {
        tree.resize(4 * SIZE);
        build(1, 0, SIZE, a);
    }

    int getSum(int l, int r)
    {
        return treeSum(1, l, r, 0, SIZE);
    }

    void changeElement(int index, int newMeaning)
    {
        change(1, index, 0, SIZE, newMeaning);
    }

    void changeSequence(int left, int right, int newMeaning)
    {
        change(1, left, right, 0, SIZE, newMeaning);
    }

private:

    struct Vertex
    {
        int sum = 0;
        bool isNeedToPush = false;
        int elements = -1;
    };

    vector<Vertex> tree;
    const int SIZE;

    void update(int v)
    {
        tree[v].sum = tree[v * 2].sum + tree[v * 2 + 1].sum;
    }

    void setFlag(int v, int l, int r, const int NEW_MEANING)
    {
        tree[v].isNeedToPush = true;
        tree[v].elements = NEW_MEANING;
        tree[v].sum = (r - l) * NEW_MEANING;
    }

    void push(int v, int l, int r)
    {
        if (r - l == 1)
            return;
        if (tree[v].isNeedToPush)
        {
            int mid = (l + r) / 2;
            setFlag(v * 2, l, mid, tree[v].elements);
            setFlag(v * 2 + 1, mid, r, tree[v].elements);
            tree[v].isNeedToPush = false;
        }
    }

    void change(int v, const int LEFT, const int RIGHT, int l, int r, const int NEW_MEANING)
    {
        push(v, l, r);
        if (l >= RIGHT || r <= LEFT)
            return;
        if (l >= LEFT && r <= RIGHT)
        {
            setFlag(v, l, r, NEW_MEANING);
            return;
        }
        int mid = (l + r) / 2;
        change(v * 2, LEFT, RIGHT, l, mid, NEW_MEANING);
        change(v * 2 + 1, LEFT, RIGHT, mid, r, NEW_MEANING);

        update(v);
    }

    void change(int v, const int POSITION, int l, int r, const int NEW_MEANING)
    {
        push(v, l, r);
        if (r - l == 1)
        {
            tree[v].sum = NEW_MEANING;
            return;
        }

        int mid = (l + r) / 2;
        if (POSITION < mid)
            change(v * 2, POSITION, l, mid, NEW_MEANING);
        else
            change(v * 2 + 1, POSITION, mid, r, NEW_MEANING);

        tree[v].sum = tree[v * 2].sum + tree[v * 2 + 1].sum;
    }

    int treeSum(int v, const int LEFT, const int RIGHT, int l, int r)
    {
        push(v, l, r);
        if (l >= RIGHT || r <= LEFT)
            return 0;
        if (l >= LEFT && r <= RIGHT)
            return tree[v].sum;

        int mid = (l + r) / 2;
        int leftPart = treeSum(v * 2, LEFT, RIGHT, l, mid);
        int rightPart = treeSum(v * 2 + 1, LEFT, RIGHT, mid, r);

        return leftPart + rightPart;
    }

    void build(int v, int left, int right, const vector<int> &arr)
    {
        if (right - left == 1)
        {
            tree[v].sum = arr[left];
            return;
        }
        int mid = (right + left) / 2;
        build(v * 2, left, mid, arr);
        build(v * 2 + 1, mid, right, arr);
        update(v);
    }
};

int main()
{
    SegmentTree tree({ 1, 2, 3, 4, 5 });
    cout << tree.getSum(0, 3) << " ";
    tree.changeElement(1, 1);
    cout << tree.getSum(0, 3) << " ";
    tree.changeSequence(0, 3, 5);
    cout << tree.getSum(0, 1) << endl;

    return 0;
}