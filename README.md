input: 
# of commands
commands..


List of Commands:
insert "Name"
search ID
search "Name"
remove ID
printInorder
printPreorder
printPostorder
printLevelCount
removeInorder N (remove the Nth gator ID from the inorder traversal of the tree (N = 0 for the first item, etc). If Nth gator ID does not exist, prints "unsuccessful". 

Example:
8
insert "David" 31239830
insert "Danielle" 27782901
insert "David" 11133245
search "Danielle"
search "David"
search 11133245
search 27782901
search 31239830


*Expected Output*
successful
successful
successful
27782901
11133245
31239830
David
Danielle
David

