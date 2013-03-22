#pragma once

#include <iostream>
#include <vector>

namespace Syntax {
	
	namespace Expressions {
		class Expression;
	}
	
	namespace Lists {
		class ParameterExpressions;
	}
	
	namespace Statements {
		
		class Statement {
			
		public:
			virtual ~Statement();
			
		};
		
		class Block : public Statement {
			
		public:
			void addStatement(Statement *stat);
			
		private:
			std::vector<Statement *> statements;
			
		};
		
		class Expression : public Statement {
			
		public:
			void setExpression(Expressions::Expression *expr);
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Import : public Statement {
			
		public:
			void setIdentifier(Expressions::Identifier *identifier);
			
		private:
			Expressions::Identifier *identifier;
			
		};
		
		class Return : public Statement {
			
		public:
			void setExpression(Expressions::Expression *expr);
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Control : public Statement {
			
		};
		
		class If : public Control {
			
		public:
			If();
			
			void setExpression(Expressions::Expression *expr);
			void setTrueStatement(Statement *statement);
			void setFalseStatement(Statement *statement);
			
		private:
			Expressions::Expression *expression;
			Statement *trueStatement, *falseStatement;
			
		};
		
		class Case : public Control {
			
		public:
			Case();
			
			void setExpression(Expressions::Expression *expr);
			void setBlock(Block *block);
			
		private:
			Expressions::Expression *expression;
			Block *block;
			
		};
		
		class Switch : public Control {
			
		public:
			void setExpression(Expressions::Expression *expr);
			void addCase(Case *statement);
			
		private:
			Expressions::Expression *expression;
			std::vector<Case *> cases;
			
		};
		
		class Declaration : public Statement {
			
		public:
			Declaration();
			
			void setExported(bool exported);
			bool getExported();
			
			void setName(Expressions::Identifier *name);
			Expressions::Identifier *getName();
			
		protected:
			bool exported;
			Expressions::Identifier *name;
			
		};
		
		class VariableDeclaration : public Declaration {
			
		public:
			VariableDeclaration();
			
			void setType(Expressions::Type *type);
			void setAssignment(Expressions::Expression *expr);
			
		private:
			Expressions::Type *type;
			Expressions::Expression *assignment;
			
		};
		
		class ConstantDeclaration : public Declaration {
			
		public:
			ConstantDeclaration();
			
			void setType(Expressions::Type *type);
			void setAssignment(Expressions::Expression *expr);
			
		private:
			Expressions::Type *type;
			Expressions::Expression *assignment;
			
		};
		
		class TypeDeclaration : public Declaration {
			
		public:
			void setType(Expressions::Type *type);
			void setBlock(Block *block);
			
		private:
			Expressions::Type *type;
			Block *block;
			
		};
		
		class FunctionDeclaration : public Declaration {
			
		public:
			void setType(Expressions::Type *type);
			Expressions::Type *getType() const;
			
			void addParameter(Expressions::Parameter *param);
			std::vector<Expressions::Parameter *> &getParameters();
			
			void setBlock(Block *block);
			
		private:
			Expressions::Type *type;
			std::vector<Expressions::Parameter *> parameters;
			Block *block;
			
		};
		
	}
	
}
