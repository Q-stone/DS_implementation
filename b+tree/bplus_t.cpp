#include "bplus_t.h"
#include <iostream>

//key 경계를 따라 value가 있거나 삽입될 bpt_data까지 탐색
bpt_data* bpt::search(int value) const {
    bpt_node* current = root;

    while (current != nullptr && !current->isLeaf()) {
        //isLeaf()가 false인 current의 child 배열에 접근
        bpt_index* index_node = (bpt_index*)current;
        int child_pos = 0;

        //value >= key[child_pos]이면 child_pos를 증가시켜 오른쪽 범위 선택
        while (child_pos < index_node->count && value >= index_node->key[child_pos]) {
            ++child_pos;
        }

        current = index_node->child[child_pos];
    }

    //current는 탐색을 마친 bpt_data 또는 빈 root의 nullptr
    return (bpt_data*)current;
}

void bpt::Insert(int value, item_data* item) {
    if (root == nullptr) {
        root = new bpt_data;
    }

    //leaf->count는 key/data 쌍의 개수; 각 data[i]의 List에 저장된 item 개수와 별개
    bpt_data* leaf = search(value);
    int insert_pos = 0;

    while (insert_pos < leaf->count && leaf->key[insert_pos] < value) {
        ++insert_pos;
    }

    //기존 key와 value가 같으면 data[insert_pos]의 List 끝에 item 추가; count 유지
    if (insert_pos < leaf->count && leaf->key[insert_pos] == value) {
        leaf->data[insert_pos]->Insert_last(item);
        return;
    }

    //key[position]과 data[position]는 한 쌍이므로 함께 이동
    for (int position = leaf->count; position > insert_pos; --position) {
        leaf->key[position] = leaf->key[position - 1];
        leaf->data[position] = leaf->data[position - 1];
    }

    leaf->key[insert_pos] = value;
    leaf->data[insert_pos] = new List<item_data*>;
    leaf->data[insert_pos]->Insert_last(item);
    ++leaf->count;

    //유효한 key는 최대 2개; count == 3이면 [a, b, c]를 [a]와 [b, c]로 분할
    if (leaf->count == 3) {
        split(leaf);
    }
}

void bpt::split(bpt_node* node) {
    bpt_node* right_node;
    int sep_key;

    if (node->isLeaf()) {
        bpt_data* left_leaf = (bpt_data*)node;
        bpt_data* right_leaf = new bpt_data;

        //left_leaf에 key[0]을 남기고 key[1], key[2]를 right_leaf로 이동
        right_leaf->count = 2;

        //data의 List 주소를 옮김; List 내부의 item 순서와 연결은 유지
        for (int position = 0; position < 2; ++position) {
            right_leaf->key[position] = left_leaf->key[position + 1];
            right_leaf->data[position] = left_leaf->data[position + 1];
            left_leaf->data[position + 1] = nullptr;
        }

        left_leaf->count = 1;

        //left_leaf와 기존 left_leaf->next 사이에 right_leaf 연결
        right_leaf->before = left_leaf;
        right_leaf->next = left_leaf->next;

        if (left_leaf->next != nullptr) {
            left_leaf->next->before = right_leaf;
        }

        left_leaf->next = right_leaf;

        //right_leaf->data는 리프에 유지; right_leaf->key[0]만 sep_key로 복사
        sep_key = right_leaf->key[0];
        right_node = right_leaf;
    }
    else {
        bpt_index* leftIndex = (bpt_index*)node;
        bpt_index* right_index = new bpt_index;

        //leftIndex->key[1]을 sep_key로 올리고 양쪽에 key 하나씩 유지
        sep_key = leftIndex->key[1];
        right_index->key[0] = leftIndex->key[2];
        right_index->count = 1;

        //leftIndex->child[2], child[3]을 right_index->child[0], child[1]로 이동
        for (int position = 0; position < 2; ++position) {
            right_index->child[position] = leftIndex->child[position + 2];
            right_index->child[position]->parent = right_index;
            leftIndex->child[position + 2] = nullptr;
        }

        leftIndex->count = 1;
        right_node = right_index;
    }

    //node->parent는 bpt_index 또는 nullptr; nullptr이면 node가 기존 root
    bpt_index* parent_idx = (bpt_index*)node->parent;

    if (parent_idx == nullptr) {
        parent_idx = new bpt_index;
        parent_idx->count = 1;
        parent_idx->key[0] = sep_key;
        parent_idx->child[0] = node;
        parent_idx->child[1] = right_node;

        node->parent = parent_idx;
        right_node->parent = parent_idx;
        root = parent_idx;
        return;
    }

    int child_pos = 0;

    while (parent_idx->child[child_pos] != node) {
        ++child_pos;
    }

    //parent_idx의 child_pos에 sep_key, child_pos + 1에 right_node를 넣을 자리 확보
    for (int position = parent_idx->count; position > child_pos; --position) {
        parent_idx->key[position] = parent_idx->key[position - 1];
    }

    for (int position = parent_idx->count + 1; position > child_pos + 1; --position) {
        parent_idx->child[position] = parent_idx->child[position - 1];
    }

    parent_idx->key[child_pos] = sep_key;
    parent_idx->child[child_pos + 1] = right_node;
    right_node->parent = parent_idx;
    ++parent_idx->count;

    if (parent_idx->count == 3) {
        split(parent_idx);
    }
}

void bpt::Delete(int value, item_data* item) {
    //value에 해당하는 key_pos를 찾고 data[key_pos]의 List에서 item 제거
    bpt_data* leaf = search(value);

    if (leaf == nullptr) {
        return;
    }

    int key_pos = 0;

    while (key_pos < leaf->count && leaf->key[key_pos] != value) {
        ++key_pos;
    }

    if (key_pos == leaf->count) {
        return;
    }

    leaf->data[key_pos]->Delete(item);

    if (!leaf->data[key_pos]->isEmpty()) {
        return;
    }

    //data[key_pos]가 비었으므로 해당 List를 해제하고 key/data 칸 제거
    delete leaf->data[key_pos];

    for (int position = key_pos; position + 1 < leaf->count; ++position) {
        leaf->key[position] = leaf->key[position + 1];
        leaf->data[position] = leaf->data[position + 1];
    }

    --leaf->count;
    leaf->data[leaf->count] = nullptr;

    if (leaf == root) {
        if (leaf->count == 0) {
            delete root;
            root = nullptr;
        }

        return;
    }

    //leaf->count == 0이면 current_idx의 인접 child에서 빌리거나 병합
    bpt_index* current_idx = (bpt_index*)leaf->parent;

    if (leaf->count == 0) {
        int child_pos = 0;

        while (current_idx->child[child_pos] != leaf) {
            ++child_pos;
        }

        bpt_data* left_nbhood = nullptr;
        bpt_data* right_nbhood = nullptr;

        if (child_pos > 0) {
            left_nbhood = (bpt_data*)current_idx->child[child_pos - 1];
        }

        if (child_pos < current_idx->count) {
            right_nbhood = (bpt_data*)current_idx->child[child_pos + 1];
        }

        if (left_nbhood != nullptr && left_nbhood->count == 2) {
            //left_nbhood->key[1]과 data[1]을 leaf의 0번 칸으로 이동
            leaf->key[0] = left_nbhood->key[1];
            leaf->data[0] = left_nbhood->data[1];
            left_nbhood->data[1] = nullptr;
            leaf->count = 1;
            left_nbhood->count = 1;
        }
        else if (right_nbhood != nullptr && right_nbhood->count == 2) {
            //right_nbhood->key[0]과 data[0]을 leaf로 이동하고 남은 칸을 앞으로 당김
            leaf->key[0] = right_nbhood->key[0];
            leaf->data[0] = right_nbhood->data[0];
            right_nbhood->key[0] = right_nbhood->key[1];
            right_nbhood->data[0] = right_nbhood->data[1];
            right_nbhood->data[1] = nullptr;
            leaf->count = 1;
            right_nbhood->count = 1;

            //right_nbhood->key[0]이 바뀌었으므로 current_idx의 해당 경계 갱신
            current_idx->key[child_pos] = right_nbhood->key[0];
        }
        else {
            //unused_leaf는 병합 후 남길 리프의 주소; 아래 delete leaf의 대상과 구분
            bpt_data* unused_leaf;
            int boundary_pos;

            if (left_nbhood != nullptr) {
                unused_leaf = left_nbhood;
                boundary_pos = child_pos - 1;
            }
            else {
                unused_leaf = right_nbhood;
                boundary_pos = 0;
            }

            //leaf->count == 0이므로 data 이동 없이 before/next 연결에서 leaf만 제거
            if (leaf->before != nullptr) {
                leaf->before->next = leaf->next;
            }

            if (leaf->next != nullptr) {
                leaf->next->before = leaf->before;
            }

            for (int position = boundary_pos; position + 1 < current_idx->count; ++position) {
                current_idx->key[position] = current_idx->key[position + 1];
            }

            for (int position = child_pos; position < current_idx->count; ++position) {
                current_idx->child[position] = current_idx->child[position + 1];
            }

            current_idx->child[current_idx->count] = nullptr;
            --current_idx->count;
            delete leaf;
            leaf = unused_leaf;
        }
    }

    //leaf->key[0]을 경계로 기록한 조상까지 changed_subt를 따라 탐색
    bpt_node* changed_subt = leaf;

    while (changed_subt->parent != nullptr) {
        bpt_index* parent_idx = (bpt_index*)changed_subt->parent;
        int child_pos = 0;

        while (parent_idx->child[child_pos] != changed_subt) {
            ++child_pos;
        }

        if (child_pos > 0) {
            parent_idx->key[child_pos - 1] = leaf->key[0];
            break;
        }

        //child_pos == 0의 최솟값은 parent_idx->key에 없으므로 상위 parent로 이동
        changed_subt = parent_idx;
    }

    //리프 병합 후 current_idx->count == 0이면 내부 노드의 재분배/병합 반복
    while (true) {
        if (current_idx == root) {
            if (current_idx->count == 0) {
                root = current_idx->child[0];
                root->parent = nullptr;

                //child[0]이 새 root가 되었으므로 current_idx 소멸 시 함께 삭제되지 않도록 분리
                current_idx->child[0] = nullptr;
                delete current_idx;
            }

            return;
        }

        if (current_idx->count > 0) {
            return;
        }

        bpt_index* parent_idx = (bpt_index*)current_idx->parent;
        int child_pos = 0;

        while (parent_idx->child[child_pos] != current_idx) {
            ++child_pos;
        }

        bpt_index* left_nbhood = nullptr;
        bpt_index* right_nbhood = nullptr;

        if (child_pos > 0) {
            left_nbhood = (bpt_index*)parent_idx->child[child_pos - 1];
        }

        if (child_pos < parent_idx->count) {
            right_nbhood = (bpt_index*)parent_idx->child[child_pos + 1];
        }

        if (left_nbhood != nullptr && left_nbhood->count == 2) {
            //parent_idx의 경계를 current_idx로 내리고 left_nbhood->child[2]를 가져옴
            current_idx->key[0] = parent_idx->key[child_pos - 1];
            current_idx->child[1] = current_idx->child[0];
            current_idx->child[0] = left_nbhood->child[2];
            current_idx->child[0]->parent = current_idx;

            //가져온 child의 시작 경계인 left_nbhood->key[1]을 parent_idx에 기록
            parent_idx->key[child_pos - 1] = left_nbhood->key[1];
            left_nbhood->child[2] = nullptr;
            current_idx->count = 1;
            left_nbhood->count = 1;
            return;
        }

        if (right_nbhood != nullptr && right_nbhood->count == 2) {
            //parent_idx의 경계를 current_idx로 내리고 right_nbhood->child[0]을 가져옴
            current_idx->key[0] = parent_idx->key[child_pos];
            current_idx->child[1] = right_nbhood->child[0];
            current_idx->child[1]->parent = current_idx;
            parent_idx->key[child_pos] = right_nbhood->key[0];

            right_nbhood->key[0] = right_nbhood->key[1];
            right_nbhood->child[0] = right_nbhood->child[1];
            right_nbhood->child[1] = right_nbhood->child[2];
            right_nbhood->child[2] = nullptr;
            current_idx->count = 1;
            right_nbhood->count = 1;
            return;
        }

        //빌릴 수 없으므로 merge_left, merge_right와 두 노드 사이의 boundary_pos 결정
        bpt_index* merge_left;
        bpt_index* merge_right;
        int boundary_pos;

        if (left_nbhood != nullptr) {
            merge_left = left_nbhood;
            merge_right = current_idx;
            boundary_pos = child_pos - 1;
        }
        else {
            merge_left = current_idx;
            merge_right = right_nbhood;
            boundary_pos = child_pos;
        }

        //l_c와 r_c는 병합 전 양쪽 key 개수; 내부 노드 병합에서는 리프의 before/next 유지
        int l_c = merge_left->count;
        int r_c = merge_right->count;

        //merge_left->key 뒤에 parent_idx->key[boundary_pos], merge_right->key 순서로 연결
        merge_left->key[l_c] = parent_idx->key[boundary_pos];

        for (int position = 0; position < r_c; ++position) {
            merge_left->key[l_c + 1 + position] = merge_right->key[position];
        }

        for (int position = 0; position <= r_c; ++position) {
            bpt_node* moved_child = merge_right->child[position];
            merge_left->child[l_c + 1 + position] = moved_child;
            moved_child->parent = merge_left;
            merge_right->child[position] = nullptr;
        }

        merge_left->count = l_c + r_c + 1;

        //parent_idx에서 사용한 key 경계 하나와 merge_right의 child 자리 제거
        for (int position = boundary_pos; position + 1 < parent_idx->count; ++position) {
            parent_idx->key[position] = parent_idx->key[position + 1];
        }

        for (int position = boundary_pos + 1; position < parent_idx->count; ++position) {
            parent_idx->child[position] = parent_idx->child[position + 1];
        }

        parent_idx->child[parent_idx->count] = nullptr;
        --parent_idx->count;
        delete merge_right;

        //parent_idx->count가 줄었으므로 다음 반복에서 parent_idx 상태 확인
        current_idx = parent_idx;
    }
}

void bpt::Print(int start, int end) const {
    bpt_data* leaf = search(start);

    //leaf 내부의 key_pos, data[key_pos]의 List, leaf->next 순서로 순회
    while (leaf != nullptr) {
        for (int key_pos = 0; key_pos < leaf->count; ++key_pos) {
            if (leaf->key[key_pos] > end) {
                return;
            }

            if (leaf->key[key_pos] < start) {
                continue;
            }

            //ListIterator<item_data*>는 저장된 item_data*의 주소인 item_data**를 반환함
            ListIterator<item_data*> iterator(*leaf->data[key_pos]);
            item_data** stored_item = iterator.First();

            while (stored_item != nullptr) {
                item_data* current_item = *stored_item;
                std::cout << leaf->key[key_pos] << " : " << current_item->itemId << '\n';
                stored_item = iterator.Next();
            }
        }

        leaf = leaf->next;
    }
}
