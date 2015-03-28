There are three new methods that I implemented in this class. 

void Update_R();
	This method will update the Rotation and Rotation Inverse Matrices.


IMPORTANT- IN THE CODE ALL CALLS TO Check_Matrix(num) , have been commented out!!!
	This is because I checked to make sure the program runs, and would catch any error during run time, but since
	now my testing phase is over, and there is no need for it now. In order to imporve performance it has been commented
	out. If you wish to uncomment it that is fine. 

void Check_Matrix(int Matrix_num) const;
	This method will check the Matrix each time it might be altered in order to aid with debugging and assure that 
	our program is correctly calculating each matrix.

bool Round_Matrix(Matrix4 & m) const;
	This check to make sure the Matrix is and Identity Matrix, by rounding and then making sure all elements along
	diagonal are 1. I had initially used a direct comparison, but this was failing a lot of the times and was causing 
	a lot of things to be printed to the screen. 





There are no know bugs after extensive testing. In addition, hopefully, the Matrix Inverse method would catch
any errors that might be created by incorrectly calculating any matrix.

This Lab I did NOT attempt extra credit.