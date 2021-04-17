#include <iostream>
#include <map>

class Polynomial {

	std::map<int, double> coefficients;
	
	void updateCoefficients();
	void updateCoefficient(int degree);

public:
	double getMonomial(int degree) const;
	void setMonomial(int degree, double value);
	
	void swap(Polynomial& polynomial1, Polynomial& polynomial2);
	
	Polynomial() {
		
	}
	
	
	Polynomial(const std::map<int, double> coefficientsMap) {
		coefficients = coefficientsMap;
		updateCoefficients();
	}
	
	
	Polynomial(double coefficientsArray[], int degree) {
		for (int i = 0; i <= degree; i++)
			coefficients[i] = coefficientsArray[i];
	}
	
	
	Polynomial(int degree, double value) {
		coefficients[degree] = value;
	}
	
	
	Polynomial(const Polynomial& polynomial2) 
	: coefficients(polynomial2.coefficients) {
		
	}
	
	
	~Polynomial() {

	}
	

	Polynomial& operator=(const Polynomial& polynomial2);
	friend bool operator==(const Polynomial& polynomial1, const Polynomial& polynomial2);
	friend bool operator!=(const Polynomial& polynomial1, const Polynomial& polynomial2);
	
	Polynomial operator+();
	Polynomial operator-();
	
	Polynomial operator+(const Polynomial& polynomial2);
	Polynomial operator-(const Polynomial& polynomial2);
	Polynomial& operator+=(const Polynomial& polynomial2);	
	Polynomial& operator-=(const Polynomial& polynomial2);

	friend Polynomial operator*(double number, const Polynomial& polynomial);
	friend Polynomial operator*(const Polynomial& polynomial, double number);
	friend Polynomial operator/(double number, const Polynomial& polynomial);
	friend Polynomial operator/(const Polynomial& polynomial, double number);
	
	friend Polynomial operator*(const Polynomial& polynomial1, const Polynomial& polynomial2);
	Polynomial& operator*=(const Polynomial& polynomial2);
	
	Polynomial& operator*=(double number);
	Polynomial& operator/=(double number);
	
	friend std::istream& operator>>(std::istream& stream, Polynomial& polynomial);
	friend std::ostream& operator<<(std::ostream& stream, Polynomial& polynomial);
	double operator[](int coefficient) const;
};






	void Polynomial::updateCoefficients() {
		if (coefficients.size() == 0)
			return;
			
		std::map<int, double>::const_iterator iter = coefficients.begin();
		
		while (iter != coefficients.end()) {
			if (iter->second == 0 || iter->first < 0)
				coefficients.erase(iter++);
			else
				iter++;
		}
	}
	
	
	void Polynomial::updateCoefficient(int degree) {
		std::map<int, double>::const_iterator find = coefficients.find(degree);
		if(find != coefficients.end() && find->second == 0)
			coefficients.erase(find);
	}

	double Polynomial::getMonomial(int degree) const {
		std::map<int, double>::const_iterator find = coefficients.find(degree);
		if (find == coefficients.end())
			return 0;
		return find->second;
	}
	
	
	
	void Polynomial::setMonomial(int degree, double value) {
		if (degree >= 0)
			coefficients[degree] = value;
		
		updateCoefficient(degree);
	}
	
	
	
	void Polynomial::swap(Polynomial& polynomial1, Polynomial& polynomial2) {
		std::swap(polynomial1.coefficients, polynomial2.coefficients);
	}
	

	

	Polynomial& Polynomial::operator=(const Polynomial& polynomial2) {
		Polynomial temp(polynomial2);
		swap(*this, temp);
	}
	
	bool operator==(const Polynomial& polynomial1, const Polynomial& polynomial2) {
		return polynomial1.coefficients == polynomial2.coefficients;
	}
	
	
	bool operator!=(const Polynomial& polynomial1, const Polynomial& polynomial2) {
		return !(polynomial1 == polynomial2);
	}
	
	Polynomial Polynomial::operator+() {
		Polynomial p(*this);
		return p;
	}
	
	
	Polynomial Polynomial::operator-() {
		Polynomial p(*this);
		for (std::map<int, double>::const_iterator iter = p.coefficients.begin(); iter != p.coefficients.end(); iter++)
			p.coefficients[iter->first] *= -1;
		return p;
	}
	
	Polynomial Polynomial::operator+(const Polynomial& polynomial2) {
		Polynomial p(*this);
		p += polynomial2;
		return p;
	}
	
	Polynomial Polynomial::operator-(const Polynomial& polynomial2) {
		Polynomial p(*this);
		p -= polynomial2;
		return p;
	}

	Polynomial& Polynomial::operator+=(const Polynomial& polynomial2) {
		for (std::map<int, double>::const_iterator iter = polynomial2.coefficients.begin(); iter != polynomial2.coefficients.end(); iter++)
			coefficients[iter->first] += polynomial2.getMonomial(iter->first);
		updateCoefficients();
		return *this;
	}
	
	Polynomial& Polynomial::operator-=(const Polynomial& polynomial2) {
		for (std::map<int, double>::const_iterator iter = polynomial2.coefficients.begin(); iter != polynomial2.coefficients.end(); iter++)
			coefficients[iter->first] -= polynomial2.getMonomial(iter->first);
		updateCoefficients();
		return *this;
	}



	Polynomial operator*(double number, const Polynomial& polynomial) {
		Polynomial p(polynomial);
		p *= number;
		return p;
	}
	
	
	Polynomial operator*(const Polynomial& polynomial, double number) {
		Polynomial p(polynomial);
		p *= number;
		return p;
	}
	
	Polynomial operator/(double number, const Polynomial& polynomial) {
		Polynomial p(polynomial);
		p /= number;
		return p;
	}
	
	
	Polynomial operator/(const Polynomial& polynomial, double number) {
		Polynomial p(polynomial);
		p /= number;
		return p;
	}
	
	Polynomial operator*(const Polynomial& polynomial1, const Polynomial& polynomial2) {
		Polynomial p;
		
		int degree;
		for (std::map<int, double>::const_iterator iter1 = polynomial1.coefficients.begin(); iter1 != polynomial1.coefficients.end(); iter1++) {
			for (std::map<int, double>::const_iterator iter2 = polynomial2.coefficients.begin(); iter2 != polynomial2.coefficients.end(); iter2++) {
				degree = iter1->first + iter2->first;
				p.setMonomial(degree, p.getMonomial(degree) + (iter1->second * iter2->second));
			}
		}
			
		return p;
	}
	
	Polynomial& Polynomial::operator*=(const Polynomial& polynomial2) {
		return *this = *this * polynomial2;
	}
	
	Polynomial& Polynomial::operator*=(double number) {
		if (number == 0) {
			coefficients.clear();
			return *this;
		}
		
		for (std::map<int, double>::const_iterator iter = coefficients.begin(); iter != coefficients.end(); iter++)
			coefficients[iter->first] *= number;
		updateCoefficients();
		return *this;
	}
	
	Polynomial& Polynomial::operator/=(double number) {
		for (std::map<int, double>::const_iterator iter = coefficients.begin(); iter != coefficients.end(); iter++)
			coefficients[iter->first] /= number;
		updateCoefficients();
		return *this;
	}
	
	
	
	std::istream& operator>>(std::istream& stream, Polynomial& polynomial) {
		int numberOfCoefficients;
		stream >> numberOfCoefficients;
	
		Polynomial p;
	
		int coefficient;
		double value;
		for (int i = 0; i < numberOfCoefficients; i++) {
			stream >> coefficient >> value;
			p.setMonomial(coefficient, value);
		}
		
		polynomial = p;
		return stream;
	}
	
	
	std::ostream& operator<<(std::ostream& stream, Polynomial& polynomial) {
		if (polynomial.coefficients.size() == 0) {
			stream << "0";
			return stream;
		}
		
		std::map<int, double>::const_iterator iter = polynomial.coefficients.begin();
		stream << iter->second << "x^" << iter->first;
		iter++;
		
		for ( ; iter != polynomial.coefficients.end(); iter++)
			stream << " + " << iter->second << "x^" << iter->first;
		
		return stream;
	}
	
	
	
	
	double Polynomial::operator[](int coefficient) const {
		std::map<int, double>::const_iterator find = coefficients.find(coefficient);
		if (find == coefficients.end())
			return 0;
		return find->second;
	}










int main() {
	
	// initialize test
	Polynomial p1;
	std::cout << "Zero: " << p1 << "\n";
	p1.setMonomial(2, 40.5);
	p1 *= 2;
	std::cout << "p1 = " << p1 << "\n\n\n";
	
	
	// == !=
	Polynomial p2(p1);
	std::cout << "p1 == p2 ? : " << (p1 == p2) << "\n";
	p2 += p1;
	std::cout << "p1 == p2 ? : " << (p1 == p2) << "\n";
	p2 -= p1;
	std::cout << "p1 == p2 ? : " << (p1 == p2) << "\n\n\n";
	
	
	// + -
	p1.setMonomial(1, 5.2);
	p2 = +p1;
	std::cout << "+p1 = " << p2 << "\n";
	p2 = -p1;
	std::cout << "-p1 = " << p2 << "\n\n\n";
	
	
	// += -=
	p1 -= p1;
	std::cout << "(p1 -= p1) = " << p1 << "\n\n\n";
	
	
	// * / *= /=
	p1.setMonomial(2, 24);
	p1.setMonomial(1, 17);
	p1.setMonomial(0, 15);
	std::cout << "p1 = " << p1 << "\n";
	p2 = p1 / 2;
	std::cout << "p1 / 2 = " << p2 << "\n";
	p1 /= 4;
	std::cout << "(p1 /= 4) = " << p1 << "\n";
	p1 *= 10;
	std::cout << "(p1 *= 10) = " << p1 << "\n";
	Polynomial p3(0, 10.0);
	std::cout << "p3 = " << p3 << "\n";
	p3.setMonomial(1, 10.0);
	p3.setMonomial(2, 25.0);
	p1 *= p3;
	std::cout << "(p1 *= p3) = " << p1 << "\n\n\n";
	
	
	// >>
	std::cout << "Enter the polynome:\n";
	std::cin >> p3;
	std::cout << "The entered polynome: " << p3 << "\n\n\n";
	
	
	// []
	std::cout << "p1 = " << p1 << "\n";
	std::cout << "p1[0] = " << p1[0] << "\n";
	std::cout << "p1[1] = " << p1[1] << "\n";
	
	
	return 0;
}
