Default login: username admin | password admin123



Rules enforced:

Menus never touch files directly — only through Services

Services never print to screen — only through Menus

FileManager is the sole file I/O point



Roles and Capabilities:

Manager: Approve/reject claims, view all reports, manage workshops, manage staff, assign surveyors

Salesman: Register customers/vehicles, issue policies, file initial claims

Surveyor: View assigned claims, submit inspection reports



Documents (docs/):

ClassDiagram.html: UML class diagram (open in browser)

UseCaseDiagram.html: UML use case diagram

cccc/:  HTML output


To Compile:
g++ -std=c++17 -Iinclude src/FileManager.cpp src/Services.cpp src/UIUtils.cpp src/ManagerMenu.cpp src/SalesmanMenu.cpp src/SurveyorMenu.cpp src/main.cpp -o insurance

