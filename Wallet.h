#pragma once

#include "OrderBookEntry.h"
#include <string> 
#include <map> 

class Wallet
{
	public: 
		Wallet();
		/** insert currenty into the wallet */
		void insertCurrency(std::string type, double amount);
		/** remove currenty from the wallet AND return with success or fail */
		bool removeCurrency(std::string type, double amount);

		/** check if the wallet  contains this much curency or more */
		bool containsCurrency(std::string type, double amount);

		/** checks if the wallet can fulfill with the current ask or bid */
		bool canFulfillOrder(OrderBookEntry order);

		/** Updates the content of the wallet
		 * assumes the order was made by the ownser of the wallet
		 */
		void processSale(OrderBookEntry& sale);

		/** generate a stirng repersentation of the wallet */
		std::string toString();
	
	private:
		std::map<std::string, double> currencies;


};