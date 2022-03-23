#include <iostream>
#include <vector>
#include <fstream>
#include <string>   
#include <cmath>
#include <sstream>
#include <iomanip>
#include<algorithm>
using namespace std;
struct pokemon {
	int ID;
	float value;
};
class RBT;
class TreeNode {
private:
	TreeNode* leftchild;
	TreeNode* rightchild;
	TreeNode* parent;
	float value;
	int ID;
	int color;             // 0= Red, 1= Black
	friend class RBT;
public:
	TreeNode() :leftchild(0), rightchild(0), parent(0), value(0), ID(0), color(0) {}
	TreeNode(float value, int ID = 0) :leftchild(0), rightchild(0), parent(0), value(value), ID(ID), color(0) {}

	float GetValue() const { return value; };
	int GetID() const { return ID; };
};

class RBT {
private:
	TreeNode* root;
	TreeNode* neel;

	void LeftRotation(TreeNode* current);
	void RightRotation(TreeNode* current);
	void InsertFixedUpRBT(TreeNode* current);
	void DeleteFixedUpRBT(TreeNode* current);
	TreeNode* Successor(TreeNode* current);
	TreeNode* Predecessor(TreeNode* current);
	TreeNode* Leftmost(TreeNode* current);
	TreeNode* Rightmost(TreeNode* current);
public:
	RBT() {
		neel = new TreeNode;    // neel �ݭn�t�m�O����
		neel->color = 1;        // neel �O�¦�
		root = neel;            // ���Finsert, �n��root ��l�Ʀ�neel
		root->parent = neel;
	};

	void InsertRBT(float value, int ID);
	void DeleteRBT(float VALUE);
	string Print(char version, int rank, float value, int number);        // �i�H�ΨӽT�{�O�_�إߦ��\


	TreeNode* Search(float VALUE);
	TreeNode* SearchK(float VALUE);
	TreeNode* FindSmallestSame(TreeNode* current);
	TreeNode* FindLargestSame(TreeNode* current);
};

void RBT::DeleteRBT(float VALUE) {              // �n�R���㦳KEY��node

	TreeNode* delete_node = Search(VALUE);   // ���T�{RBT���O�_�s�b�㦳KEY��node
	if (delete_node == NULL) {
		//std::cout << "data not found.\n";
		return;
	}

	TreeNode* y = 0;     // �u���n�Q�R��������O���骺node
	TreeNode* x = 0;     // �n�Q�R����node��"child"

	if (delete_node->leftchild == neel || delete_node->rightchild == neel) {
		y = delete_node;
	}
	else {
		y = Successor(delete_node);         // �Ny�]��delete_node��Successor
	}                                       // �g�L�o��if-else, y�վ㦨�ܦh�u���@��child

	if (y->leftchild != neel) {              // �Nx�]��y��child, �i�঳��ڸ��, �]���i��ONIL
		x = y->leftchild;
	}
	else {
		x = y->rightchild;
	}

	x->parent = y->parent;                 // �Y��x�ONIL�]�n��x��parent���V���Ī��O�����m
										   // �]���bFixUp�ɻݭn�ǥ�x->parent�P�_x��leftchild�άOrightchild

	if (y->parent == neel) {                // ���ۦA��n�Q����O���骺node��"parent"���V�s��child
		this->root = x;                    // �Y�R�����O�����root, �N��x���s��root
	}
	else if (y == y->parent->leftchild) {   // �Yy�쥻�O��parent��left child
		y->parent->leftchild = x;          // �K��x�Ҧby��parent��left child, ���Ny
	}
	else {                                  // �Yy�쥻�O��parent��right child
		y->parent->rightchild = x;         // �K��x�Ҧby��parent��right child, ���Ny
	}

	if (y != delete_node) {                // �w��case3
		delete_node->value = y->value;         // �Yy�Odelete_node������, �̫�n�A�Ny�����
		delete_node->ID = y->ID; // ��^delete_node���O�����m, �ñNy���O�����m����
	}

	if (y->color == 1) {                   // �Y�R����node�O�¦�, �n�qx�i��ץ�, �H�ŦXRBT���C��W�h
		DeleteFixedUpRBT(x);
	}


}
void RBT::DeleteFixedUpRBT(TreeNode* current) {
	// Case0:(i)  current�O���⪺, �����⥦���
	//       (ii) current�Oroot, �����⥦���
	while (current != root && current->color == 1) {
		// current�Oleftchild
		if (current == current->parent->leftchild) {
			TreeNode* sibling = current->parent->rightchild;
			// Case1: �p�Gsibling�O����
			if (sibling->color == 0) {
				sibling->color = 1;
				current->parent->color = 0;
				LeftRotation(current->parent);
				sibling = current->parent->rightchild;
			}
			// �i�J Case2�B3�B4: sibling�O�¦�
			// Case2: sibling�����child���O�¦�
			if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
				sibling->color = 0;
				current = current->parent;           // �Ycurrent��s��root, �Y���X�j��
			}
			// Case3 & 4: current�u���䤤�@��child�O�¦�
			else {
				// case3: sibling��right child�O�ª�, left child�O����
				if (sibling->rightchild->color == 1) {
					sibling->leftchild->color = 1;
					sibling->color = 0;
					RightRotation(sibling);
					sibling = current->parent->rightchild;
				}
				// �g�LCase3��, �@�w�|�ܦ�Case4
				// Case 4: sibling��right child �O���⪺, left child�O�¦�
				sibling->color = current->parent->color;
				current->parent->color = 1;
				sibling->rightchild->color = 1;
				LeftRotation(current->parent);
				current = root;     // �Ncurrent���ʨ�root, �@�w���X�j��
			}
		}
		// current�Orightchild
		else {
			TreeNode* sibling = current->parent->leftchild;
			// Case1: �p�Gsibling�O����
			if (sibling->color == 0) {
				sibling->color = 1;
				current->parent->color = 0;
				RightRotation(current->parent);
				sibling = current->parent->leftchild;
			}
			// �i�J Case2�B3�B4: sibling�O�¦�
			// Case2: sibling�����child���O�¦�
			if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
				sibling->color = 0;
				current = current->parent;             // �Ycurrent��s��root, �Y���X�j��
			}
			// Case3 & 4: current�u���䤤�@��child�O�¦�
			else {
				// case3: sibling��left child�O�ª�, right child�O����
				if (sibling->leftchild->color == 1) {
					sibling->rightchild->color = 1;
					sibling->color = 0;
					LeftRotation(sibling);
					sibling = current->parent->leftchild;
				}
				// �g�LCase3��, �@�w�|�ܦ�Case4
				// Case 4: sibling��left child �O���⪺, rightt child�O�¦�
				sibling->color = current->parent->color;
				current->parent->color = 1;
				sibling->leftchild->color = 1;
				RightRotation(current->parent);
				current = root;     // �Ncurrent���ʨ�root, �@�w���X�j��
			}
		}
	}
	current->color = 1;
}
void RBT::InsertRBT(float value, int ID) {
	// �e�b���P InsertBST()���޿觹���ۦP, �ȶȻݭn�ק� NULL <-> NIL
	TreeNode* y = neel;
	TreeNode* x = root;
	TreeNode* insert_node = new TreeNode(value, ID);

	while (x != neel) {     // ��root��l�Ʀ�neel, �o�̴N�i�H��neel�Ӱ��P�_
		y = x;
		if (insert_node->value < x->value) {
			x = x->leftchild;
		}
		else if (insert_node->value > x->value) {
			x = x->rightchild;
		}
		else {
			if (insert_node->ID > x->ID) {
				x = x->leftchild;
			}
			else {
				x = x->rightchild;

			}
		}
	}

	insert_node->parent = y;

	if (y == neel) {
		this->root = insert_node;
	}
	else if (insert_node->value < y->value) {
		y->leftchild = insert_node;
	}
	else if (insert_node->value > y->value) {
		y->rightchild = insert_node;
	}
	else {  //�۵������p ��ID �p���b�e
		if (insert_node->ID > y->ID) {
			y->leftchild = insert_node;
		}
		else {
			y->rightchild = insert_node;
		}
	}

	// �H�U�O��RBT��node���]�w, �Nchild pointer���VNIL, �C��]������
	insert_node->leftchild = neel;
	insert_node->rightchild = neel;
	insert_node->color = 0;             // �C��i�H�bconstructor���w�]

	InsertFixedUpRBT(insert_node);      // ��i��X�{����P����node�۳s�����ΰ��ץ�
}

void RBT::InsertFixedUpRBT(TreeNode* current) {

	// case0: parent�O�¦�, �N���ζi�^��
	while (current->parent->color == 0) {   // �Yparent�O����Y�i�J�j��

		// �W�b���Gparent�Ograndparent��left child
		if (current->parent == current->parent->parent->leftchild) {
			TreeNode* uncle = current->parent->parent->rightchild;
			// case1: �Yuncle�O����
			if (uncle->color == 0) {
				current->parent->color = 1;
				uncle->color = 1;
				current->parent->parent->color = 0;              //grandparent�令����
				current = current->parent->parent;
			}
			// case2 & 3: uncle�O�¦�
			else {
				if (current == current->parent->rightchild) {     // case2
					current = current->parent;
					LeftRotation(current);
				}
				// case3
				current->parent->color = 1;                      //��parent���
				current->parent->parent->color = 0;              // grandparent���
				RightRotation(current->parent->parent);
			}
		}
		// �U�b���Gparent�Ograndparent��right child, �P�W�b�����
		else {
			TreeNode* uncle = current->parent->parent->leftchild;
			// case1: �Yuncle�O����
			if (uncle->color == 0) {
				current->parent->color = 1;
				uncle->color = 1;
				current->parent->parent->color = 0;              //grandparent�令����
				current = current->parent->parent;
			}
			// case2 & 3: uncle�O�¦�
			else {
				if (current == current->parent->leftchild) {     // case2
					current = current->parent;
					RightRotation(current);
				}
				// case3
				current->parent->color = 1;
				current->parent->parent->color = 0;
				LeftRotation(current->parent->parent);
			}
		}
	}
	root->color = 1;    // �T�Oroot�O�¦�
}

void RBT::LeftRotation(TreeNode* x) {

	TreeNode* y = x->rightchild;         // ��y���Vx��rightchild, �̫�y�|�ܦ�x��parent

	x->rightchild = y->leftchild;        // �ϤG(c)��, ��y��leftchild���bx��rightchild

	if (y->leftchild != neel) {           // �ϤG(c)�k, �Ynode(j)��NIL�h����
		y->leftchild->parent = x;        // �N���y��leftchild��parent�令x
	}

	y->parent = x->parent;               // �ϤG(d)��, ��sy��parent

	if (x->parent == neel) {              // �ϤG(d)�k, �Y���x�Oroot, �����y�ܦ��s��root
		root = y;
	}
	else if (x == x->parent->leftchild) { // �Y���x�O��parent��leftchild
		x->parent->leftchild = y;        // ��s��y�]�O��parent��leftchild
	}
	else {                                // �Y���x�O��parent��rightchild
		x->parent->rightchild = y;       // ��s��y�]�O��parent��rightchild
	}
	y->leftchild = x;                    // �ϤG(e)��, �̫�~�ק�y�Px
	x->parent = y;                       // �ϤG(e)�k
}
void RBT::RightRotation(TreeNode* y) {

	TreeNode* x = y->leftchild;           // ��x�]��y��leftchild

	y->leftchild = x->rightchild;         // ��x��rightchild���y��leftchild    
	if (x->rightchild != neel) {           // �Yx��rightchild����NIL, �N��parent���Vy
		x->rightchild->parent = y;
	}
	x->parent = y->parent;                // �Nx��parent���V���y��parent
										  // �H�U�@��if-else�N�ק���y��parent��child
	if (y->parent == neel) {               // �Yy����Oroot, x�N�����s��root
		root = x;
	}
	else if (y == y->parent->leftchild) {  // �Y���y�O��parent��leftchild, 
		y->parent->leftchild = x;         // x�N������s��parent��leftchild
	}
	else {                                 // �Y���y�O��parent��rightchild, 
		y->parent->rightchild = x;        // x�N������s��parent��rightchild
	}
	x->rightchild = y;                    // �Ny�]��x��rightchild
	y->parent = x;                        // �Nx�]��y��parent
}

TreeNode* RBT::Leftmost(TreeNode* current) {

	while (current->leftchild->ID != 0) {
		current = current->leftchild;
	}
	return current;
}

TreeNode* RBT::Rightmost(TreeNode* current) {

	while (current->rightchild->ID != 0) {
		current = current->rightchild;
	}
	return current;
}

TreeNode* RBT::Successor(TreeNode* current) {

	if (current->rightchild->ID != 0) {
		return Leftmost(current->rightchild);
	}

	TreeNode* new_node = current->parent;

	while (new_node->ID != 0 && current == new_node->rightchild) {
		current = new_node;
		new_node = new_node->parent;
	}

	return new_node;
}

TreeNode* RBT::Predecessor(TreeNode* current) {

	if (current->leftchild->ID != 0) {
		return Rightmost(current->leftchild);
	}

	TreeNode* new_node = current->parent;

	while (new_node->ID != 0 && current == new_node->leftchild) {
		current = new_node;
		new_node = new_node->parent;
	}
	return new_node;
}



TreeNode* RBT::Search(float VALUE) {

	TreeNode* current = root;               // �Ncurent���Vroot�@��traversal�_�I

	while (current->ID != 0 && VALUE != current->value) {  // ��ر��p���X�j��G
													  // 1.�S��� 2.�����
		if (VALUE < current->value) {
			current = current->leftchild;   // �V����
		}
		else {
			current = current->rightchild;  // �V�k��
		}
	}
	return current;
}

TreeNode* RBT::SearchK(float VALUE) {

	TreeNode* current = root;



	while (current->ID != 0 && VALUE != current->value) {

		if (VALUE < current->value && current->leftchild->ID != 0) {
			current = current->leftchild;
		}
		else if (VALUE > current->value && current->rightchild->ID != 0) {
			current = current->rightchild;
		}
		else if (VALUE == current->value) {
			current = current;
		}
		else
		{
			if (current->value < VALUE) {
				current = Predecessor(current); //�T�O���O�ۦP �N�O�U�@��
			}
			return current;
		}
	}
	return current;
}

TreeNode* RBT::FindSmallestSame(TreeNode* current) {

	while (Successor(current)->value == current->value) {
		current = Successor(current);
	}
	return current;
}

TreeNode* RBT::FindLargestSame(TreeNode* current) {

	while (Predecessor(current)->value == current->value) {
		current = Predecessor(current);
	}
	return current;
}

bool comp(const pokemon &a,const pokemon &b)
{
return a.value < b.value;
}


string RBT::Print(char version, int rank, float value, int number) {

	TreeNode* current = root;
  string result = ""; 
  stringstream buffer;
	int K_output = number;
	vector<int> pokemonID;
	vector<struct pokemon> small;
	vector<struct pokemon> large;
  vector<struct pokemon> all;
	struct pokemon temp;
	TreeNode* K;
	TreeNode* S;
	TreeNode* L;
	int tree_node_number = 0;
	int counter;
	int smallest_same;
  int LARGE=0, SMALL=0;
	int number_s, number_l;
	int count_s, count_l;
	while (current->leftchild->ID != 0) {
		current = current->leftchild;
	}
	tree_node_number++;
  
	while (Successor(current)->ID != 0) {
		current = Successor(current);
		tree_node_number++;
	}

	switch (version) {

	case 'r':
		rank = rank - 1; //�{���q0�}�l��
		current = Leftmost(root); //����̤p
		counter = tree_node_number; //�̤p���� �̫�@�W
		smallest_same = FindSmallestSame(current)->ID; //�̫�@�W��ID
		if (counter == rank) {
			//cout << "r " << current->ID << " " << current->value << endl;
        buffer <<  "r " << current->ID << " " <<fixed<< setprecision(1)<<current->value << endl;
		}
		if (Successor(current)->value != current->value) { //�U�@�ӤH���԰��O�@�˪��ܴN���Χ�F ���P�A��
			smallest_same = FindSmallestSame(Successor(current))->ID; //4
		}
		counter--;
		while (Successor(current)->ID != 0) {
			if (counter == rank) {
				//cout << "r " << smallest_same << " " << current->value << endl;
        buffer <<  "r " << smallest_same << " " <<fixed<< setprecision(1)<<current->value << endl;
			}
			if (Successor(current)->value != current->value) { //�U�@�ӤH���԰��O�@�˪��ܴN���Χ�F ���P�A��
				smallest_same = FindSmallestSame(Successor(current))->ID;
			}
			current = Successor(current);
			counter--;
		}
   
    result = buffer.str();
		break;

	case 'R':
		rank = rank - 1; //�{���q0�}�l��
		current = Leftmost(root); //����̤p
		counter = tree_node_number; //�̤p���� �̫�@�W
		if (counter == rank) {
			buffer << "R " << current->ID << " " << fixed<<setprecision(1)<<current->value << endl;
		}
		counter--;
		while (Successor(current)->ID != 0) {
			if (counter == rank) {
				buffer << "R " << current->ID << " " << fixed<<setprecision(1)<<current->value << endl;
			}
			current = Successor(current);
			counter--;
		}
   result = buffer.str();
		break;

	case 'v':


    buffer << "v ";
		rank = tree_node_number;
		current = Leftmost(root); //����̤p
		if (current->value == value) {
			pokemonID.push_back(rank);
		}
		rank--;
		while (Successor(current)->ID != 0) {
			current = Successor(current);
			if (current->value == value) {
				pokemonID.push_back(rank);
			}
			rank--;
		}
		buffer << pokemonID.back() << endl ;
		//buffer << pokemonID.front() << endl;
    result = buffer.str();
		break;




	case 'V':
		buffer << "V ";
		rank = tree_node_number;
		current = Leftmost(root); //����̤p
		if (current->value == value) {
			pokemonID.push_back(rank);
		}
		rank--;
		while (Successor(current)->ID != 0) {
			current = Successor(current);
			if (current->value == value) {
				pokemonID.push_back(rank);
			}
			rank--;
		}
		buffer << pokemonID.back() << " ";
		buffer << pokemonID.front() << endl;
    result = buffer.str();
		break;

	case 'K': //K <value> <number>��X�P�S�w�԰��O�ƭȶZ���e N�Ӫ�԰��O
 
   if (value < Leftmost(root)->value){
   
     value = Leftmost(root)->value;
   }
 
		K = SearchK(value);
     number--;
		// K�i��O�ۦP�Τj�@�I
		buffer << "K ";
		//FindSmallestSame(�̥k�䪺) FindLargestSame(�̥��䪺) Predecessor(���@��) Successor(�k�@��)
		//���ۦP���N���o�۪����L�F
		if (K->value == value) {
			K = FindSmallestSame(K);
			buffer << K->ID << " ";
			number--;
			while (Predecessor(K)->value == K->value && number >= 0) {
				K = Predecessor(K);
				buffer << K->ID << " ";
				number--;
			}
			L = Successor(FindSmallestSame(K));//K���k�@�ӼƦr
			S = Predecessor(FindLargestSame(K));//K�����@�ӼƦr
		}
		else {
			L = K;//�S��K �L���ӴN�O�k��Ʀr
			S = Predecessor(FindLargestSame(K));//K�����@�ӼƦr
		}
		//�Ы�small vector �x�snumber�Ӥ�K�p��
		number_s = number;
		while (number_s >= 0 && Predecessor(S)->ID != 0) {
			temp.ID = S->ID;
			temp.value = abs(S->value - value);
			small.push_back(temp);
      SMALL++;
			S = (Predecessor(S));
			number_s--;
		}
		temp.ID = S->ID;
		temp.value = abs(S->value - value);
		small.push_back(temp);
    SMALL++;
		number_l = number;
		L = FindSmallestSame(L);
		temp.ID = L->ID;
		temp.value = abs(L->value - value);
		large.push_back(temp);
    LARGE++;
		number_l--;

		while (number_l >= 0) {
			if (Predecessor(L)->value == L->value) {
				L = (Predecessor(L));
				temp.ID = L->ID;
				temp.value = abs(L->value - value);
				large.push_back(temp);
        LARGE++;
				number_l--;
			}
			else if (Successor(FindSmallestSame(L))->ID != 0) { //�k���٦��󰪧����O�� �N���L�h
				L = Successor(FindSmallestSame(L));
				L = FindSmallestSame(L);
				temp.ID = L->ID;
				temp.value = abs(L->value - value);
				large.push_back(temp);
        LARGE++;
				number_l--;
			}
			else //�S�F
			{
				number_l = -1;
			}
		}

		count_l = 0;
		count_s = 0;
		while (number >= 0)
		{

      
			if (count_s == SMALL && number > 0) { //small �Χ��F
        while(number>=0){
    				//buffer << large[count_l].ID << " ";
            all.push_back(large[count_l]);
    				count_l++;
            number--;
        }
			}
			else if (count_l == LARGE && number > 0) { //large�Χ��F
      
        while(number>=0){
    				//buffer << small[count_s].ID << " ";
            all.push_back(small[count_s]);
    				count_s++;
            number--;
          }
        
			}
				if (large[count_l].value > small[count_s].value && number >= 0) {
					//buffer << small[count_s].ID << " ";
          all.push_back(small[count_s]);
					count_s++;
          number--;
				}
				else if (large[count_l].value < small[count_s].value && number >= 0) {
					//buffer << large[count_l].ID << " ";
                 all.push_back(large[count_l]);
					count_l++;
          number--;
				}
				else if (large[count_l].value == small[count_s].value && number >= 0) { //�۵��L�X�p��

					if (large[count_l].ID < small[count_s].ID) {
						//buffer << large[count_l].ID << " ";
            all.push_back(large[count_l]);
						count_l++;
            number--;
					}
					else {
						//buffer << small[count_s].ID << " ";
            all.push_back(small[count_s]);
						count_s++;
            number--;
					}
				}

		}
   
   //����vector


    sort(all.begin(),all.end(),comp);

    for(int i =0;i<all.size();i++){
      buffer << all[i].ID<<" ";
    }





    //
    result = buffer.str();
    result.erase(result.find_last_not_of(" ")+1);
    //result.append("\n");
    
    //cout << result;
    
    
		break;
	}
return result;
}
int main(int argc, char* argv[]) {
	RBT tree;
	fstream input(argv[1]);
	ofstream output(argv[2]);
  string result;
	char token;
	int ID, rank, number;
	float value; //�԰��O
	//input Ū��
	while (input >> token) {
		switch (token) {
		case 'I':
			input >> ID;
			input >> value;
			//insert
			tree.InsertRBT(value, ID);
			break;
		case 'D':
			input >> ID;
			input >> value;
			//delete
			tree.DeleteRBT(value);
			break;
		case 'r':
			input >> rank;
			//print rank v1
			output << tree.Print('r', rank, 0, 0);
			break;
		case 'R':
			input >> rank;
			//print rank v2
			output << tree.Print('R', rank, 0, 0);
			break;
		case 'v':
			input >> value;
			//print value v1
			output << tree.Print('v', 0, value, 0);
			break;
		case 'V':
			input >> value;
			//print value v2
			output << tree.Print('V', 0, value, 0);
			break;
		case 'K':
			input >> value;
			input >> number;
			//print k
			output << tree.Print('K', 0, value, number);
			break;
		}
	}
	input.close();
	output.close();
	return 0;
}
