#pragma once

#include "BitFunnel/NonCopyable.h"
#include "BitFunnel/Plan/RowMatchNode.h"
#include "BitFunnel/Plan/TermMatchNode.h"
#include "BitFunnel/Term.h"  // needed for maxGramSize.
// #include "BitFunnel/Stream.h"


namespace BitFunnel
{
    class IAllocator;
    class ISimpleIndex;
    class PlanRows;
    template <typename T, size_t LOG2_CAPACITY>
    class RingBuffer;

    class TermMatchTreeConverter : NonCopyable
    {
    public:
        TermMatchTreeConverter(const ISimpleIndex& index,
                               PlanRows& planRows,
                               // bool generateNonBodyPlan,
                               IAllocator& allocator);

        const RowMatchNode& BuildRowMatchTree(const TermMatchNode& root);

    private:
        const RowMatchNode* BuildMatchTree(const TermMatchNode& node);
        const RowMatchNode* BuildMatchTree(const TermMatchNode::And& node);
        const RowMatchNode* BuildMatchTree(const TermMatchNode::Not& node);
        const RowMatchNode* BuildMatchTree(const TermMatchNode::Or& node);
        const RowMatchNode* BuildMatchTree(const TermMatchNode::Phrase& node);
        const RowMatchNode* BuildMatchTree(const TermMatchNode::Unigram& node);
        const RowMatchNode* BuildMatchTree(const TermMatchNode::Fact& node);

        // Builds a RowMatchNode for a soft-deleted document row. This row
        // excludes documents which are marked as soft-deleted, from matching.
        const RowMatchNode* BuildDocumentActiveMatchNode();

        const Term GetUnigramTerm(char const * text, Term::StreamId streamId) const;
        void ProcessNGramBuffer(RowMatchNode::Builder& builder,
                                RingBuffer<Term, Term::c_log2MaxGramSize + 1>& termBuffer);
        void AppendTermRows(RowMatchNode::Builder& builder, const Term& term);
        void AppendTermRows(RowMatchNode::Builder& builder, const FactHandle& fact);

        IAllocator& m_allocator;
        const ISimpleIndex& m_index;
        PlanRows& m_planRows;

        // A flag to indicate if NonBodyQueryPlan is requested to be generated.
        // bool m_generateNonBodyPlan;
    };
}
