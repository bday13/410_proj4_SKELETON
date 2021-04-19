#include <mutex>

#include "../includes/baker.h"
#include "../includes/constants.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	Box b;
	int count = 0;
	for (int i = 0; i < anOrder.number_donuts; i++) {
		count++;
		DONUT d;
		b.addDonut(d);

		if (count % DOZEN == 0) {
			anOrder.boxes.push_back(b);
			b.clear();
			count = 0;
		}
	}
	anOrder.boxes.push_back(b);
	
}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {
	ORDER order;
	while (!b_WaiterIsFinished || !order_in_Q.empty()) {
		unique_lock<mutex> lock(mutex_order_inQ);
		while (order_in_Q.size() == 0 && !b_WaiterIsFinished) {
			cv_order_inQ.wait(lock);
		}
		if (!order_in_Q.empty()) {
			order = order_in_Q.front();
			order_in_Q.pop();
			bake_and_box(order);
			lock_guard<mutex> lock(mutex_order_outQ);
			order_out_Vector.push_back(order);
		}
	}
}
