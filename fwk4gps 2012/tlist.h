#ifndef TLIST_H
#define TLIST_H
template<class T>
class TList;

template<class T>
class Node{
	friend class TList<T>;
	T data_;
	Node<T>* next_;
public:
	Node(const T& data){
		data_=data;
		next_=0;
	}
	const T& data(){return data_;}
};
template<class T>
class TList{
	Node<T>* start_;
	Node<T>* end_;
	Node<T>* curr_;
	
public:
	TList(){start_=end_=curr_=0;}
	Node<T>* start()const {return start_;}
	Node<T>* curr() const{return curr_;}
	void insertAtFront(const T& data);
	void insertAtBack(const T& data);
	void removeAtFront();
	void removeCurrent();
	void goStart(){curr_=start_;}
	void goNext(){curr_=curr_?curr_->next_:0;}
	~TList();
};
template<class T>
void TList<T>::insertAtFront(const T& data){
	Node<T>* nn=new Node<T>(data);
	if(!start_){
		start_=end_=nn;
	}
	else{
		nn->next_=start_;
		start_=nn;
	}
}
template<class T>
void TList<T>::insertAtBack(const T& data){
	Node<T>* nn=new Node<T>(data);
	if(!start_){
		start_=end_=nn;
	}
	else{
		end_=nn;
	}
}
template<class T>
void TList<T>::removeAtFront(){
	Node<T>* rm=start_;
	if(start_){
		start_=start_->next_;
		delete rm;
	}
}
template<class T>
void TList<T>::removeCurrent(){
	Node<T>* rm=curr_;
	if(curr_){
		if(curr_==start_){
			if(curr_==end_)
				end_=0;
			start_=start_->next_;
			curr_=start_;
		}
		else{
			Node<T>* prev=start_;
			while(prev->next_!=curr_){
				prev=prev->next_;
			}
			prev->next_=curr_->next_;
			if(curr_==end_){
				end_=prev;
				curr_=0;
			}
			else{
				curr_=curr_->next_;
			}
		}
		delete rm;
	}
	
}
template<class T>
TList<T>::~TList(){
	curr_=start_;
	Node<T>* rm;
	while(curr_){
		rm=curr_;
		curr_=curr_->next_;
		delete rm;
	}
}
#endif