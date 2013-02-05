var mongoose = require('mongoose');

function DBManager(mongodbPath)
{
	//mongoose.connect(mongodbPath);

	var Schema = mongoose.Schema;
	var ObjectId = Schema.ObjectID;

	var Friend = new Schema({
		name: { type: String, required: true, trim: true }
	});

	var User = new Schema({
		first_name:	{ type: String, trim: true },
		last_name:	{ type: String, trim: true },
		username:	{ type: String, trim: true },
		ip:			{ type: String, trim: true },
		friends:	[Friend],
		score:		{ type: Number },
		games:		{ type: Number }
	});
	var User_M = mongoose.model('User',User);

//	test = new User_M();
//	test.username = 'blah';
//	test.save( function(error, data){
//		if(error){
//			console.log(error);
//		}
//		else{
//			console.log(data);
//		}
//	});
//
}

module.exports = DBManager;
