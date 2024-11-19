#include <algorithm>
#include "pubsub.h"

using namespace std;

map<string, vector<Subscriber*>> PubSub::subscribers;

void PubSub::subscribe(string channel, Subscriber* subscriber)
{
	if (subscribers.find(channel) == subscribers.end())
		subscribers[channel] = vector<Subscriber*>();

	vector<Subscriber*>& callbacks = subscribers[channel];

	if (find(callbacks.begin(), callbacks.end(), subscriber) == callbacks.end())
		callbacks.push_back(subscriber);
}

void PubSub::publish(string channel, string message, void* data)
{
	if (subscribers.find(channel) == subscribers.end())
		return;

	// Don't use a reference here, because it's entirely possible that
	// while this message is being processed subscribers might be added
	// to this channel, which would crash the program.
	vector<Subscriber*> callbacks = subscribers[channel];

	for (Subscriber* subscriber : callbacks)
		subscriber->receiveMessage(channel, message, data);
}

void PubSub::unsubscribe(string channel, Subscriber* subscriber)
{
	if (subscribers.find(channel) == subscribers.end())
		return;

	vector<Subscriber*>& callbacks = subscribers[channel];
	vector<Subscriber*>::iterator result = find(callbacks.begin(), callbacks.end(), subscriber);

	if (result != callbacks.end())
		callbacks.erase(result);
}
